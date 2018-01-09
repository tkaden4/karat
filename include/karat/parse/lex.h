#pragma once
/* stdlib files */
#include<wchar.h>
#include<stdlib.h>
/* karat files */
#include<karat/parse/rbuff.h>
#include<setjmp.h>

/* TODO remove limits on lexeme size */
/* use ungetc() to read entire file, interspersing L'\0' */
#define MAX_LEXEME 20

#define MAX_LEX_LOOK 4
#if MAX_LEX_LOOK <= 3 
#error "Lexer lookahead must be above 3"
#endif

struct token {
    wchar_t lexeme[MAX_LEXEME];
    size_t lsize;
    /* token type */
    enum {
        TOK_ID,
        TOK_NUM,
        TOK_REG,
        TOK_ADDR,
        TOK_COMMA,
        TOK_COLON,
        TOK_EOL,
        TOK_EOS,
    } type;
    unsigned line_no;
    unsigned col_no;
    /* parsed data */
    /* TODO move out of lexer */
    long long data;
};

struct lex_state {
    /* pointer to file */
    FILE *file;
    /* buffer for lookahead chars */
    RBUFF_DECL(lex_la_buff, wchar_t, MAX_LEX_LOOK) la_chars;
    /* debugging info */
    unsigned line_no;
    unsigned col_no;
    /* TODO remove */
    jmp_buf *err;
};

void lex_init(struct lex_state *state, FILE *f, jmp_buf *b);
int lex_next(struct lex_state *state, struct token *tok);
