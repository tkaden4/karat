#pragma once
#include<wchar.h>
#include<stdlib.h>

#include"types.h"

struct token {
	#define MAX_LEXEME 50
	wchar_t lexeme[MAX_LEXEME];
	enum {
		TOK_ID,
		TOK_NUM,
		TOK_ADDR,
		TOK_REG,
		TOK_COLON,
		TOK_COMMA,
	} type;
	long long data;	/* for num and reg */
	addr_t addr;	/* for addr */
};

int tokenize_line(const wchar_t *line, struct token **toks, size_t *res_size);
