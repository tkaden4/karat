#pragma once

/* stdlib files */
#include<stdio.h>
#include<setjmp.h>
/* karat files */
#include<vm/kprog.h>
#include<list.h>
#include<parse/lex.h>
#include<parse/rbuff.h>

#define MAX_LOOK 2

struct label_arg {
	SLINK(struct label_arg);
	wchar_t *id;
	addr_t rpos;	/* location to resolve */
};

struct label_def {
	addr_t pos;	/* what position it points to */
};

struct parse_state {
	struct lex_state lstate;
	/* MAX_LOOK tokens will always be in buffer */
	RBUFF_DECL(tok_la_buff, struct token, MAX_LOOK) la_buff;
	/* current position in bytecode */
	size_t cur_insns;
	/* output program */
	struct kprog *prog;
	/* list of label arguments */
	struct label_arg *label_args;
	/* label definitions */
	struct smap *label_defs;
	/* error handling point */
	jmp_buf err_ex;
};

/* parse file into program */
int parse_file(FILE *f, struct kprog *res);
