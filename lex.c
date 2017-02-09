#include<wctype.h>
#include<ctype.h>
#include<string.h>

#include"alloc.h"
#include"lex.h"
#include"log.h"

struct lex_state {
	FILE *file;
	wchar_t la;
	/* debugging info */
	size_t line_no;
	size_t col_no;
} state;

void lex_init(FILE *f)
{
	state.file = f;
	state.la = fgetwc(f);
	state.line_no = 0;
	state.col_no = 0;
}

static wchar_t la()
{
	return state.la;
}

static void advance()
{
	wint_t la = fgetwc(state.file);
	if(la == WEOF){
		state.la = L'\0';
	}else{
		state.la = la;
	}
}

static int is_id_body(wint_t c)
{
	return iswalpha(c) || c == L'_';
}

static inline int lex_number(struct token *res, size_t base)
{
	size_t i = 0;
	wchar_t c = 0;
	while(iswdigit((c = la()))){
		if(i == MAX_LEXEME - 1){
			break;
		}
		res->lexeme[i++] = c;
		advance();
	}
	res->data = wcstoll(res->lexeme, NULL, base);
	return 0;
}

int lex_next(struct token *res)
{
	err_on(!res, "Token not allocated");
	while(iswspace(la())){
		advance();
	}

	if(la() == L';'){
		while(la() && la() != L'\n'){
			advance();
		}
	}

	memset(res->lexeme, 0, sizeof(wchar_t)*MAX_LEXEME);

	#define CASEC(c, t)\
		case c: advance(); res->lexeme[0] = c; res->type = t; break;

	switch(la()){
	case L'\n':
		res->type = TOK_EOL;
		wcsncpy(res->lexeme, L"EOL", MAX_LEXEME);
		advance();
		break;
	case L'\0':
		res->type = TOK_EOS;
		wcsncpy(res->lexeme, L"EOS", MAX_LEXEME);
		break;
	case L'#':
		res->type = TOK_NUM;
		advance();
		switch(la()){
		case L'$': advance(); lex_number(res, 16); break;
		case L'%': advance(); lex_number(res, 2); break;
		default:
			if(iswdigit(la())){
				lex_number(res, 10);
			}else{
				return 1;
			}
		};
		break;
	case L'$':
		res->type = TOK_ADDR;
		advance();
		lex_number(res, 16);
		break;
	case L'r':
		res->type = TOK_REG;
		res->lexeme[0] = L'r';
		advance();
		size_t i = 1;
		wchar_t c = 0;
		while(iswdigit((c = la()))){
			if(i == MAX_LEXEME - 1){
				break;
			}
			res->lexeme[i++] = c;
			advance();
		}
		res->data = wcstoll(res->lexeme + 1, NULL, 10);
		break;
	CASEC(L':', TOK_COLON)
	CASEC(L',', TOK_COMMA)
	default:
		if(is_id_body(la())){
			res->type = TOK_ID;
			wchar_t c = 0;
			size_t index = 0;
			while(is_id_body((c = la()))){
				if(index == MAX_LEXEME - 2){
					break;
				}
				res->lexeme[index++] = c;
				advance();
			}
		}else{
			err("couldn't match character %lc", la());
		}
		break;
	};
	return 0;
}
