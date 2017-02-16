#pragma once
/* stdlib files */
#include<wchar.h>
#include<stdlib.h>
/* karat files */
#include<parse/srbuff.h>
#include<ktypes.h>

struct token {
	#define MAX_LEXEME 20
	wchar_t lexeme[MAX_LEXEME];
	size_t lsize;
	enum {
		TOK_ID = 1,
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
	#define MAX_LEX_LOOK 10
	RBUFF_DECL(lex_la_buff, wchar_t, MAX_LEX_LOOK) la_chars;
	/* debugging info */
	unsigned line_no;
	unsigned col_no;
};

void lex_init(struct lex_state *state, FILE *f);
int lex_next(struct lex_state *state, struct token *tok);
void lex_destroy(struct lex_state *state);
