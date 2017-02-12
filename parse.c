#include"parse.h"

#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>

#include"lex.h"
#include"util.h"
#include"alloc.h"
#include"log.h"
#include"smap.h"

#define MAX_LOOK 10 

struct fwdef {
	SLINK(struct fwdef);	/* next forward declaration in list */
	addr_t *pos;			/* location to resolve */
};

struct label_def {
	struct fwdef *fwdefs;
	addr_t pos;				/* what position it points to */
};

static void destroy_def(void *ptr)
{
	struct label_def *def = ptr;
	LIST_FREELOOP(struct fwdef, def->fwdefs, x){
		s_free(x);
	}
	s_free(def);
}

/* deal with all forward declarations */
static void resolve_fwdefs(struct label_def *def)
{
	LIST_FOREACH(struct fwdef, def->fwdefs, x){
		*(x->pos) = def->pos;
	}
}

struct parse_state {
	/* parsing utils */
	struct lex_state lstate;
	/* MAX_LOOK tokens will always be in buffer */
	struct token la_buff[MAX_LOOK];
	size_t cur_size;
	size_t index;
	/* for generation */
	struct kprog *prog;	/* output program */
	size_t cur_insns;	/* current position in bytecode */
	/* map of labels to addresses */ 
	struct smap *label_defs;
	/* map of labels to forward declarations */
	struct smap *fwdefs;
};

/* get the actual index of the nth element in the buffer */
static inline size_t rbuff_elem_index(struct parse_state *state, long long n)
{
	return (state->index + n) % MAX_LOOK;
}

/* add an element to the end of the buffer */
static inline void rbuff_push_back(struct parse_state *state)
{
	const size_t index = rbuff_elem_index(state, state->cur_size);
	lex_next(&state->lstate, &state->la_buff[index]);
	++state->cur_size;
}

/* remove an element from the end of the buffer */
static inline void rbuff_pop_back(struct parse_state *state)
{
	--state->cur_size;
}

/* remove an element from the front of the buffer */
static inline void rbuff_pop_front(struct parse_state *state)
{
	state->index = rbuff_elem_index(state, 1);
	--state->cur_size;
}

static void parse_init(struct parse_state *state, struct kprog *prog, FILE *f)
{
	memset(state->la_buff, 0, sizeof(struct token)*MAX_LOOK);
	state->cur_size = 0;
	state->index = 0;

	lex_init(&state->lstate, f);

	for(size_t i = 0; i < MAX_LOOK; ++i){
		rbuff_push_back(state);
	}

	state->prog = prog;
	state->cur_insns = 0;
	state->label_defs = smap_create(destroy_def);
}

static void parse_destroy(struct parse_state *ctx)
{
	smap_destroy(ctx->label_defs);
}

static int parse_test_n(struct parse_state *state, size_t n, unsigned tok_type)
{
	if(n >= MAX_LOOK){
		return 0;
	}
	return state->la_buff[rbuff_elem_index(state, n)].type == tok_type;
}

static int parse_test(struct parse_state *state, unsigned tok_type)
{
	return parse_test_n(state, 0, tok_type);
}

static void parse_advance(struct parse_state *state)
{
	/* delete front element from buffer */
	rbuff_pop_front(state);
	/* read new element into buffer */
	rbuff_push_back(state);
}

static const struct token *parse_la_n(struct parse_state *state, size_t n)
{
	(void) parse_test;
	return &state->la_buff[rbuff_elem_index(state, n)];
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
	wchar_t *label = wcsdup(parse_la(state)->lexeme);
	parse_match(state, TOK_ID);
	parse_match(state, TOK_COLON);
	if(!wcscmp(label, KPROG_ENTRY_POINT)){
		if(state->prog->entry_point){
			err("multiple definitions of entry point");
			return 1;
		}else{
			puts("found entry point");
			state->prog->entry_point = &state->prog->program[state->cur_insns];
		}
	}else{
		printf("encountered label %ls:\n", label);
		/* TODO resolve any forward declaration */
		struct label_def *def = smap_lookup(state->label_defs, label);
		if(def){
			if(def->fwdefs){
				resolve_fwdefs(def);
			}else{
				/* TODO error, label has now been redefined */
				return 1;
			}
		}else{	/* no label yet */
			struct label_def *def = s_alloc(struct label_def);
			smap_insert(state->label_defs, label, def);
		}
	}
	s_free(label);
	return 0;
}

static int parse_ins(struct parse_state *state)
{
	/* I should probably duplicate this string */
	const wchar_t *op_str = parse_la(state)->lexeme;
	printf("opcode %ls\n", op_str);
	parse_match(state, TOK_ID);

	/* parse arguments */
	const struct token *la = parse_la(state);
	switch(parse_la(state)->type){
	case TOK_REG:	/* register argument */
	{
		return 1;
	}
	case TOK_NUM:	/* number argument */
	{
		return 1;
	}
	case TOK_ID:	/* label argument */
	{
		struct label_def *def = NULL;
		if((def = smap_lookup(state->label_defs, la->lexeme))){
			/* argument is address of label */
		}else{
			/* TODO add as forward declaration */
			printf("adding forward decl to %ls\n", la->lexeme);
		}
	}
	case TOK_EOL:
	default: return 1;
	};
}

static int parse_expr(struct parse_state *state)
{
	switch(parse_la(state)->type){
	case TOK_ID:
		if(parse_test_n(state, 1, TOK_COLON)){
			if(parse_label(state)){
				return 1;
			}else{
				return parse_expr(state);
			}
		}else{
			if(parse_ins(state)){
				return 1;
			}
			parse_match(state, TOK_EOL);
			return 0;
		}
	case TOK_EOL: parse_advance(state); return 0;
	default: return 1;
	};
}

int parse_file(FILE *f, struct kprog *res)
{
	err_on(!f, "input file not opened");
	err_on(!res, "output program not allocated");
	struct parse_state state;
	parse_init(&state, res, f);

	int ret = 0;

	const struct token *la = parse_la(&state);
	while(la->type != TOK_EOS){
		ret = parse_expr(&state);
		if(ret){
			break;
		}
		la = parse_la(&state);
	}
	/* TODO check_fwdefs(state) */
	parse_destroy(&state);
	return ret;
}
