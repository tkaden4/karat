/* stdlib files */
#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>
/* karat files */
#include<parse/rbuff.h>
#include<parse/parse.h>
#include<parse/lex.h>
#include<list.h>
#include<ktypes.h>
#include<util.h>
#include<alloc.h>
#include<log.h>
#include<smap.h>

/* create a stack copy of a string */
#define STACK_WCSDUP(str, from) \
	wchar_t str[wcslen(from) + 1]; \
	wcscpy(str, from);

RBUFF_IMPL(tok_la_buff, struct token, MAX_LOOK);

static inline void *current_byte(struct parse_state *state)
{
	return &state->prog->program[state->cur_insns];
}

static void prog_write(struct parse_state *state, u32 data, size_t bytes)
{
	(void) state;
	bytes = bytes > sizeof(data) ? sizeof(data) : bytes;
	register const u8 *bp = (u8 *)&data;
	for(register size_t i = 1; i <= bytes; ++i){
		printf("0x%X\n", bp[bytes - i]);
	}
}
/* adds a label at the current position
 * returns 1 on redefinition of label */
static int add_label_def(struct parse_state *state, const wchar_t *str, addr_t pos)
{
	if(smap_lookup(state->label_defs, str)){
		return 1;
	}
	struct label_def *def = s_alloc(struct label_def);
	def->pos = pos;
	smap_insert(state->label_defs, str, def);
	return 0;
}

/* adds a label argument to the label argument list
 * at the current position, with argument size asize */
static void add_label_arg(struct parse_state *state, const wchar_t *str)
{
	struct label_arg *arg = s_alloc(struct label_arg);
	arg->rpos = current_byte(state);
	arg->id = wcsdup(str);
	/* add argument to beginning of list */
	arg->next = state->label_args;
	state->label_args = arg;
}

static int resolve_label_arguments(struct parse_state *state)
{
	int err = 0;
	LIST_FREELOOP(struct label_arg, state->label_args, each){
		struct label_def *label = smap_lookup(state->label_defs, each->id);
		if(!label){
			err = 1;
			printf("label %ls never defined in source\n", each->id);
		}
		s_free(each->id);
		s_free(each);
	}
	state->label_args = NULL;
	return err;
}

static void parse_init(struct parse_state *state, struct kprog *prog, FILE *f)
{
	tok_la_buff_init(&state->la_buff);
	lex_init(&state->lstate, f);

	for(size_t i = 0; i < MAX_LOOK; ++i){
		struct token *t = tok_la_buff_push_back(&state->la_buff);
		lex_next(&state->lstate, t);
	}

	state->prog = prog;
	state->cur_insns = 0;
	state->label_args = NULL;
	state->label_defs = smap_create_d();
}

static void parse_destroy(struct parse_state *state)
{
	/* deal with unhandled forward definitions */
	smap_destroy(state->label_defs);
	lex_destroy(&state->lstate);
}

static const struct token *parse_la(struct parse_state *state);

static int parse_test_n(struct parse_state *state, size_t n, unsigned tok_type)
{
	if(n >= MAX_LOOK){
		return 0;
	}
	return tok_la_buff_elem_n(&state->la_buff, n)->type == tok_type;
}

static int parse_test(struct parse_state *state, unsigned tok_type)
{
	return parse_test_n(state, 0, tok_type);
}

static void parse_advance(struct parse_state *state)
{
	/* delete front element from buffer */
	tok_la_buff_pop_front(&state->la_buff);
	lex_next(&state->lstate, tok_la_buff_push_back(&state->la_buff));
}

static const struct token *parse_la_n(struct parse_state *state, size_t n)
{
	return tok_la_buff_elem_n(&state->la_buff, n);
}

static const struct token *parse_la(struct parse_state *state)
{
	return parse_la_n(state, 0);
}

static void parse_match(struct parse_state *state, unsigned tok_type)
{
	if(parse_la(state)->type != tok_type){
		err("couldn't match target type on \"%ls\"", parse_la(state)->lexeme);
	}else{
		parse_advance(state);
	}
}


static int parse_label(struct parse_state *state)
{
	STACK_WCSDUP(label, parse_la(state)->lexeme);
	parse_match(state, TOK_ID);
	parse_match(state, TOK_COLON);

	if(!wcscmp(label, KPROG_ENTRY_POINT)){
		if(state->prog->entry_point){
			puts("multiple definitions of entry point");
			return 1;
		}else{
			state->prog->entry_point = state->cur_insns;
		}
	}
	if(add_label_def(state, label, state->cur_insns)){
		printf("redefinition of label %ls\n", label);
	}
	return 0;
}

/* TODO deal with preprocessing */
/* TODO how to embed argument types into bytecode */
static int parse_arg(struct parse_state *state, size_t op_size)
{
	const struct token *la = parse_la(state);
	switch(la->type){
	case TOK_REG:	/* register argument */
	case TOK_NUM:	/* number argument */
	case TOK_ADDR:
		/* XXX */
		state->cur_insns += op_size;
		parse_advance(state);
		break;
	case TOK_ID:	/* label argument */
		add_label_arg(state, la->lexeme);
		parse_advance(state);
		break;
	case TOK_EOL:
	default:
		return 1;
	};
	return 0;
}

static int parse_args(struct parse_state *state, size_t op_size)
{
	while(!parse_arg(state, op_size) && parse_test(state, TOK_COMMA)){
		parse_match(state, TOK_COMMA);
	}
	return 0;
}

static size_t get_arg_size(wchar_t c)
{
	switch(c){
	case L'l': return 4;
	case L'w': return 2;
	case L'b': return 1;
	default: return 0;
	};
}


static int parse_ins(struct parse_state *state)
{
	STACK_WCSDUP(op_str, parse_la(state)->lexeme);
	parse_match(state, TOK_ID);

	unsigned num_args = 0;
	u8 arg_byte = 0;
	/* TODO move into genscript */

#define opcode(mn, b, nargs) \
	if(!wcscmp(mn, op_str)){ \
		arg_byte = b; \
		num_args = nargs; \
	} else
#include"vm/inc/opcodes.inc"
	{
		printf("opcode undefined: \"%ls\"\n", op_str);
		return 1;
	}
#undef opcode

	debug("found opcode \"%ls\" with %u args, byte 0x%02X",
			op_str, num_args, arg_byte);

	if(num_args == 0){
		return 0;
	}else if(parse_test(state, TOK_DOT_CHAR)){
		const wchar_t size_char = parse_la(state)->lexeme[1];
		size_t arg_size = 0;
		if(!(arg_size = get_arg_size(size_char))){
			printf("no size \'%lc\'\n", size_char);
			return 1;
		}
		parse_advance(state);
		return parse_args(state, arg_size);
	}else{
		puts("expected operand size");
		return 1;
	}
}

/* parse a single valid line of assembly */
static int parse_expr(struct parse_state *state)
{
	switch(parse_la(state)->type){
	case TOK_ID:
		if(parse_test_n(state, 1, TOK_COLON)){
			if(parse_label(state)){
				return 1;
			}else{
				/* labels may be followed by more labels and one opcode */
				return parse_expr(state);
			}
		}else{
			if(parse_ins(state)){
				return 1;
			}
			/* only one instruction allowed per line */
			parse_match(state, TOK_EOL);
			return 0;
		}
	case TOK_EOL:
		parse_advance(state);
		return 0;
	default:
		return 1;
	};
}

int parse_file(FILE *f, struct kprog *res)
{
	err_on(!f, "input file not opened");
	err_on(!res, "output program not allocated");

	struct parse_state state;
	parse_init(&state, res, f);

	int ret = 0;
	const struct token *la = NULL;
	while(!ret && (la = parse_la(&state))->type != TOK_EOS){
		ret = parse_expr(&state);
	}
	resolve_label_arguments(&state);
	parse_destroy(&state);
	return ret;
}
