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

#define parse_err(state, fmt, ...) \
{ \
	struct lex_state *lstate = &state->lstate; \
	printf("[ error ] on line %u, column %u :", lstate->line_no, lstate->col_no); \
	printf(" " fmt "\n", ##__VA_ARGS__); \
	longjmp(state->err_ex, 1); \
}

#define parse_warn(fmt, ...) \
{ \
	printf("[ warning ] :"); \
	printf(" " fmt "\n", ##__VA_ARGS__); \
}

static size_t reserve_byte(struct parse_state *state)
{
	size_t ret = state->cur_insns;
	kprog_append_bytes(state->prog, 0, 1);
	return ret;
}
static size_t reserve_word(struct parse_state *state)
{
	size_t ret = state->cur_insns;
	kprog_append_bytes(state->prog, 0, 2);
	return ret;
}
static size_t reserve_long(struct parse_state *state)
{
	size_t ret = state->cur_insns;
	kprog_append_bytes(state->prog, 0, 4);
	return ret;
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
	arg->rpos = reserve_long(state);
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
			parse_warn("label %ls never defined in source", each->id);
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
		parse_err(	state, "couldn't match target type on \"%ls\"",
					parse_la(state)->lexeme);
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
			parse_err(state, "multiple definitions of entry point");
		}else{
			state->prog->entry_point = state->cur_insns;
		}
	}
	if(add_label_def(state, label, state->cur_insns)){
		parse_err(state, "redefinition of label %ls", label);
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
		parse_err(state, "opcode undefined: \"%ls\"\n", op_str);
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
			parse_err(state, "no size \'%lc\'", size_char);
		}
		parse_advance(state);
		return parse_args(state, arg_size);
	}else{
		parse_err(state, "expected operand size");
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

int parse_file(FILE *in_f, struct kprog *res_prog)
{
	err_on(!in_f, "input file not opened");
	err_on(!res_prog, "output program not allocated");

	struct parse_state state;
	parse_init(&state, res_prog, in_f);

	int ret = 0;
	/* set up error handling */
	if(!setjmp(state.err_ex)){
		const struct token *la = NULL;
		while(!ret && (la = parse_la(&state))->type != TOK_EOS){
			ret = parse_expr(&state);
		}
		resolve_label_arguments(&state);
	}else{	/* error specific code */
		ret = 1;
		LIST_FREELOOP(struct label_arg, state.label_args, each){
			s_free(each->id);
			s_free(each);
		}
	}
	parse_destroy(&state);
	return ret;
}
