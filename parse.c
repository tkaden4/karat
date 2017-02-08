#include"parse.h"

#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>

#include"lex.h"
#include"util.h"
#include"alloc.h"
#include"log.h"
#include"cpu.h"
#include"opcode.h"

#define min(a, b) ((a) > (b)) ? (b) : (a)

/* error handling */
static struct {
	/* line number */
	size_t line_no;
	/* line itself */
	const wchar_t *line;
	/* error data */
	const wchar_t * perr_string;
} err_info = {1, L"", L"No error"};

#define parse_err(msg, ...) \
	do{ \
		err("Parse error : " msg, ##__VA_ARGS__); \
	}while(0);

#define TRY(exp) \
	do { \
		int e = exp; \
		if(e){ FATAL_ERR(e); } \
	}while(0);

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

struct parse_ctx {
	struct token *toks;
	size_t ntoks;
	size_t index;
};

void parse_ctx_init(struct parse_ctx *ctx, struct token *toks, size_t ntoks)
{
	ctx->toks = toks;
	ctx->ntoks = ntoks;
	ctx->index = 0;
}

int parse_test(struct parse_ctx *ctx, unsigned tok_type)
{
	if(ctx->index == ctx->ntoks){
		return 0;
	}
	return ctx->toks[ctx->index].type == tok_type;
}

int parse_test_n(struct parse_ctx *ctx, size_t n, unsigned tok_type)
{
	if(ctx->index + n >= ctx->ntoks){
		return 0;
	}
	return ctx->toks[ctx->index + n].type == tok_type;
}

void parse_advance(struct parse_ctx *ctx)
{
	if(ctx->index == ctx->ntoks){
		err("not enough tokens in line");
	}
	++ctx->index;
}

int parse_has_next(struct parse_ctx *ctx)
{
	if(ctx->index == ctx->ntoks){
		return 0;
	}
	return 1;
}

const struct token *parse_get(struct parse_ctx *ctx)
{
	return &ctx->toks[ctx->index];
}

int parse_arguments(struct parse_ctx *ctx, struct arg_data res[2])
{
	res[0].type = ARG_NONE;
	res[1].type = ARG_NONE;
	if(!parse_has_next(ctx)){
		return 0;
	}
	struct arg_data *cur_arg = res;
	/* TODO finish */
eval_arg:
	switch(parse_get(ctx)->type){
	case TOK_ADDR:	/* address argument */
		cur_arg->type = ARG_ADDRESS;
		cur_arg->aval = parse_get(ctx)->data;
		parse_advance(ctx);
		break;
	case TOK_NUM:	/* number argument */
		cur_arg->type = ARG_LONG;
		cur_arg->ival = parse_get(ctx)->data;
		parse_advance(ctx);
		break;
	case TOK_ID:	/* label argument */
		cur_arg->type = ARG_LABEL;
		/* TODO duplicate this */
		cur_arg->lval = parse_get(ctx)->lexeme;
		parse_advance(ctx);
		break;
	case TOK_REG:
		cur_arg->type = ARG_REGISTER;
		cur_arg->rval = parse_get(ctx)->data;
		parse_advance(ctx);
		break;
	default:
		return 1;
	};
	if(parse_has_next(ctx)){
		if(parse_test(ctx, TOK_COMMA)){
			parse_advance(ctx);
			if(cur_arg == &res[1]){
				return 1;
			}
			cur_arg = &res[1];
			goto eval_arg;
		}else{
			return 1;
		}
	}
	return 0;
}

/* returns 1 on a malformed line */
int evaluate_tokens(struct token *toks, size_t ntoks, struct presult *res)
{
	if(ntoks == 0){
		return 0;
	}
	struct parse_ctx ctx;
	parse_ctx_init(&ctx, toks, ntoks);
	if(!parse_test(&ctx, TOK_ID)){
		return 1;
	}
	if(parse_test_n(&ctx, 1, TOK_COLON)){
		const wchar_t *label = parse_get(&ctx)->lexeme;
		printf("encountered label \"%ls:\"\n", label);
		/* TODO ACTUALLY */
		parse_advance(&ctx);
		parse_advance(&ctx);
	}else{
		const wchar_t *opcode = parse_get(&ctx)->lexeme;
		printf("encountered opcode \"%ls\"", opcode);
		parse_advance(&ctx);
		if(parse_arguments(&ctx, res->u.op.args)){
			return 1;
		}
		printf(" with args");
		#define parg(x) \
			switch(x.type){\
			case ARG_LONG: printf(" %d", x.ival); break;\
			case ARG_ADDRESS: printf(" %d", x.aval); break;\
			case ARG_REGISTER: printf(" %d", x.rval); break;\
			case ARG_LABEL: printf(" %ls", x.lval); break;\
			case ARG_NONE: printf(" none"); break;\
			default: printf(" any");\
			};
		parg(res->u.op.args[0]);
		printf(" and");
		parg(res->u.op.args[1]);
		printf("\n");
	}
	return 0;
}

int parse_file(FILE *f, struct kprog *res)
{
	err_on(!f, "input file not opened");
	err_on(!res, "output program not allocated");
	wchar_t buff[100] = {L'\0'};
	while(fgetws(buff, 100, f)){
		err_info.line = buff;
		struct token *toks = NULL;
		size_t res = 0;
		tokenize_line(buff, &toks, &res);
		if(res == 0){
			/* ignore lines with nothing */
		}else{
			struct presult res_p;
			evaluate_tokens(toks, res, &res_p);
		}
		++err_info.line_no;
	}
	return 0;
}

const wchar_t *get_perr()
{
	return err_info.perr_string;
}
