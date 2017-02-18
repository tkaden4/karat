/* stdlib files */
#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>
/* karat files */
#include<parse/rbuff.h>
#include<parse/parse.h>
#include<parse/lex.h>
#include<vm/opcode.h>
#include<vm/cpu.h>
#include<list.h>
#include<ktypes.h>
#include<alloc.h>
#include<log.h>
#include<smap.h>

/* create a stack copy of a string */
#define STACK_WCSDUP(str, from) \
	wchar_t str[wcslen(from) + 1]; \
	wcscpy(str, from);

static const struct token *parse_la(struct parse_state *state);

/* inform user of fatal error
 * and jump to exception handler */
#define parse_err(state, fmt, ...) \
{ \
	printf("[  error  ] :"); \
	printf(" " fmt "\n", ##__VA_ARGS__); \
	longjmp((state)->err_ex, 1); \
}

/* warn user of something non-fatal */
#define parse_warn(fmt, ...) \
{ \
	printf("[ warning ] :"); \
	printf(" " fmt "\n", ##__VA_ARGS__); \
}

/* reserve bytes in memory, returning
 * the location where it was reserved */
#define RESERVE_FUNC(name, type) \
static size_t reserve_ ## name(struct parse_state *state) \
{ \
	size_t ret = state->cur_insns; \
	kprog_append_bytes(state->prog, 0, sizeof(type)); \
	state->cur_insns += sizeof(type); \
	return ret; \
}

/* write bytes to the program memory */
#define WRITE_FUNC(name, type) \
static void write_ ## name(struct parse_state *state, type b, size_t at) \
{ \
	/* TODO unsafe, defer to kprog -> insert_bytes */ \
	memcpy(state->prog->program + at, &b, sizeof(type)); \
}

/* create implementation of token lookahead buffer */
RBUFF_IMPL(tok_la_buff, struct token, MAX_LOOK);

/* create memory-reserving functions */
RESERVE_FUNC(byte, u8);
RESERVE_FUNC(word, u16);
RESERVE_FUNC(long, u32);

/* create writing functions */
WRITE_FUNC(byte, u8);
WRITE_FUNC(word, u16);
WRITE_FUNC(long, u32);

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

/* calculates label offets
 * and places them where arguments are needed */
static void resolve_label_arguments(struct parse_state *state)
{
	int err = 0;
	LIST_FREELOOP(struct label_arg, state->label_args, each){
		struct label_def *label = smap_lookup(state->label_defs, each->id);
		write_long(state, label->pos, each->rpos);
		if(!label){
			err = 1;
			parse_warn("label %ls never defined in source", each->id);
		}
		s_free(each->id);
		s_free(each);
	}
	if(err){
		parse_err(state, "undefined labels in program");
	}
	state->label_args = NULL;
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

static int parse_test_2(struct parse_state *state, unsigned tok_1, unsigned tok_2)
{
	return parse_test(state, tok_1) && parse_test_n(state, 1, tok_2);
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
	if(!parse_test_2(state, TOK_ID, TOK_COLON)){
		parse_err(state, "mangled label");
	}
	STACK_WCSDUP(label, parse_la(state)->lexeme);
	if(!wcscmp(label, KPROG_ENTRY_POINT)){
		if(state->prog->entry_point >= 0){
			parse_err(state, "multiple definitions of entry point");
		}else{
			state->prog->entry_point = state->cur_insns;
		}
	}
	if(add_label_def(state, label, state->cur_insns)){
		parse_err(state, "redefinition of label %ls", label);
	}
	parse_match(state, TOK_ID);
	parse_match(state, TOK_COLON);
	return 0;
}

/* TODO deal with preprocessing */
/* TODO how to embed argument types into bytecode */
static int parse_arg(struct parse_state *state, u8 *spec)
{
	const struct token *la = parse_la(state);
	switch(la->type){
	case TOK_REG:	/* register argument */
		if(la->data >= GENERAL_REGS){
			parse_err(state, "no register named \"%ls\"\n", la->lexeme);
		}
		*spec = 0;
		write_long(state, la->data, reserve_long(state));
		break;
	case TOK_NUM:	/* number argument */
		*spec = 1;
		write_long(state, la->data, reserve_long(state));
		break;
	case TOK_ADDR:	/* address argument */
		*spec = 2;
		write_long(state, la->data, reserve_long(state));
		break;
	case TOK_ID:	/* label argument */
		*spec = 1;
		add_label_arg(state, la->lexeme);
		break;
	case TOK_EOL:
	default:
		return 1;
	};
	parse_advance(state);
	return 0;
}

static const struct op_def *find_def(const wchar_t *wcs)
{
	for(size_t i = 0; i < sizeof(op_defs)/sizeof(op_defs[0]); ++i){
		if(!wcscmp(op_defs[i].mnemonic, wcs)){
			return (op_defs + i);
		}
	}
	return NULL;
}

static int parse_ins(struct parse_state *state)
{
	STACK_WCSDUP(op_str, parse_la(state)->lexeme);
	parse_match(state, TOK_ID);
	/* TODO re-implement opcode parsing */
	const struct op_def *op = find_def(op_str);
	if(op){
	
	}else{
		parse_err(state, "unrecognized opcode \"%ls\"", op_str);
	}
	return 0;
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
	/* handles a blank line */
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
		if(res_prog->entry_point < 0){
			parse_err(&state, "entry point not defined");
		}
		kprog_finalize(res_prog);
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
