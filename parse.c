#include"parse.h"

#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>

#include"lex.h"
#include"util.h"
#include"alloc.h"
#include"log.h"
#include"opcode.h"
#include"smap.h"

struct presult;

struct label_data {
	struct presult *next;
	#define MAX_LABEL 20
	wchar_t label[MAX_LABEL];
};

struct opcode_data {
	size_t mnemonic_num;
	struct arg_data {
		enum {
			ARG_LABEL,
			ARG_ADDRESS,
			ARG_LONG,
			ARG_REGISTER,
			ARG_NONE,
		} type;
		union {
			const wchar_t *lval;
			/* constant values */
			addr_t aval;
			i32 ival;
			unsigned rval;
		};
	} args[2];	/* arguments (0 for none) */
};

/* parsed line data */
struct presult {
	enum {
		RES_LABEL_DECL,
		RES_OPCODE,
	} type;
	union {
		struct label_data label;
		struct opcode_data op;
	} u;
};

static inline void reset_presult(struct presult *res)
{
	res->type = -1;
	memset(&res->u, 0, sizeof(res->u));
}


struct label_def {
	int is_fwdef;
	size_t pos;
};

#define MAX_LOOK 4

struct parse_ctx {
	/* parsing utils */
	struct token la_buff[MAX_LOOK];
	size_t cur_size;

	/* for generation */
	struct kprog *prog;	/* output program */
	size_t cur_insns;	/* current position in bytecode */

	/* map of labels to addresses */ 
	struct smap *label_defs;
};

void parse_ctx_init(struct parse_ctx *ctx, struct kprog *prog)
{
	memset(ctx->la_buff, 0, sizeof(struct token)*MAX_LOOK);
	ctx->cur_size = 0;

	ctx->prog = prog;
	ctx->cur_insns = 0;
	ctx->label_defs = smap_create();
}

void parse_ctx_destroy(struct parse_ctx *ctx)
{
	smap_destroy(ctx->label_defs);
}

int parse_test_n(struct parse_ctx *ctx, size_t n, unsigned tok_type)
{
	(void) ctx;
	(void) n;
	(void) tok_type;
	return 0;
}

int parse_test(struct parse_ctx *ctx, unsigned tok_type)
{
	return parse_test_n(ctx, 0, tok_type);
}

void parse_advance(struct parse_ctx *ctx)
{
	(void) ctx;
}

int parse_has_next(struct parse_ctx *ctx)
{
	(void) ctx;
	return 0;
}

const struct token *parse_get(struct parse_ctx *ctx)
{
	(void) ctx;
	return NULL;
}

int parse_file(FILE *f, struct kprog *res)
{
	err_on(!f, "input file not opened");
	err_on(!res, "output program not allocated");
	struct parse_ctx ctx;
	parse_ctx_init(&ctx, res);
	return 0;
}
