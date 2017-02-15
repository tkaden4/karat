#pragma once
/* stdlib files */
#include<wchar.h>
#include<stdlib.h>
/* karat files */
#include<ktypes.h>

struct token {
	#define MAX_LEXEME 20
	wchar_t lexeme[MAX_LEXEME];
	size_t lsize;
	enum {
		TOK_ID,
		TOK_NUM,
		TOK_REG,
		TOK_ADDR,
		TOK_COMMA,
		TOK_COLON,
		TOK_DOT_CHAR,	/* .l, .d, .w */
		TOK_EOL,
		TOK_EOS,
	} type;
	long long data;	/* for num and reg, as well as dot_char */
};

struct lex_state {
	FILE *file;
	wchar_t la;
	/* debugging info */
	size_t line_no;
	size_t col_no;
};

void lex_init(struct lex_state *state, FILE *f);
int lex_next(struct lex_state *state, struct token *tok);
