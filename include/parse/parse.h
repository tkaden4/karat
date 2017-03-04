#pragma once

/* stdlib files */
#include<stdio.h>
#include<setjmp.h>
/* karat files */
#include<vm/kprog.h>
#include<list.h>
#include<parse/lex.h>
#include<parse/rbuff.h>

#define MAX_LOOK 3

struct label_def {
	addr_t pos;	/* what position it points to */
};

struct label_arg;
struct parse_state;
typedef int(*resolve_f)(const struct label_def *,
						struct label_arg *,
						struct parse_state *);
struct label_arg {
	SLINK(struct label_arg);
	wchar_t *id;
	size_t op_pos;
	/* how to resolve the argument */
	resolve_f resolve;
};

struct parse_state {
	struct lex_state lstate;
	/* MAX_LOOK tokens will always be in buffer */
	RBUFF_DECL(tok_la_buff, struct token, MAX_LOOK) la_buff;
	/* current position in bytecode */
	size_t cur_insns;
	/* current complete opcode */
	size_t cur_op;
	/* output program */
	struct kprog *prog;
	/* list of label arguments */
	struct label_arg *label_args;
	/* label definitions */
	struct smap *label_defs;
	/* error handling point */
	jmp_buf err_handler;
};

/* parse file into program */
int parse_file(FILE *f, struct kprog *res);
