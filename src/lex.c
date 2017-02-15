#include<wctype.h>
#include<ctype.h>
#include<string.h>
#include<parse/lex.h>
#include<alloc.h>
#include<log.h>

void lex_init(struct lex_state *state, FILE *f)
{
	state->file = f;
	state->la = fgetwc(f);
	state->line_no = 0;
	state->col_no = 0;
}

static wchar_t la(struct lex_state *state)
{
	return state->la;
}

static void advance(struct lex_state *state)
{
	wint_t la = fgetwc(state->file);
	if(la == WEOF){
		state->la = L'\0';
	}else{
		++state->col_no;
		if(la == L'\n'){
			++state->line_no;
			state->col_no = 0;
		}
		state->la = la;
	}
}

static int is_id_start(wint_t c)
{
	return iswalpha(c) || c == L'_';
}

static int is_id_body(wint_t c)
{
	return iswalpha(c) || iswdigit(c) || c == '_';
}

static int iswbin(wint_t c)
{
	return c == L'1' || c == L'0';
}

static inline int lex_number(
	struct lex_state *state,
	struct token *res,
	size_t base)
{
	static int(*test)(wint_t) = NULL;
	switch(base){
	case 0:
	case 10:
		test = iswdigit;
		break;
	case 2:
		test = iswbin;
		break;
	case 16:
		test = iswxdigit;
		break;
	default:
		return 1;
	};
	size_t i = 0;
	wchar_t c = 0;
	while(test((c = la(state)))){
		if(i == MAX_LEXEME - 1){
			break;
		}
		res->lexeme[i++] = c;
		advance(state);
	}
	res->data = wcstoll(res->lexeme, NULL, base);
	return 0;
}

int lex_next(struct lex_state *state, struct token *res)
{
	err_on(!res, "Token not allocated");
	while(la(state) == L'\t' || la(state) == L' '){
		advance(state);
	}

	if(la(state) == L';'){
		while(la(state) && la(state) != L'\n'){
			advance(state);
		}
	}

	memset(res, 0, sizeof(struct token));

	#define CASEC(c, t)\
		case c: advance(state); res->lexeme[0] = c; res->type = t; break;

	switch(la(state)){
	case L'\n':
		res->type = TOK_EOL;
		wcsncpy(res->lexeme, L"EOL", MAX_LEXEME);
		advance(state);
		break;
	case L'\0':
		res->type = TOK_EOS;
		wcsncpy(res->lexeme, L"EOS", MAX_LEXEME);
		break;
	case L'.':
		advance(state);
		if(iswalpha(la(state))){
			advance(state);
		}
		break;
	case L'#':
		res->type = TOK_NUM;
		advance(state);
		switch(la(state)){
		case L'$': advance(state); lex_number(state, res, 16); break;
		case L'%': advance(state); lex_number(state, res, 2); break;
		default:
			if(iswdigit(la(state))){
				lex_number(state, res, 10);
			}else{
				return 1;
			}
		};
		break;
	case L'$':
		res->type = TOK_ADDR;
		advance(state);
		lex_number(state, res, 16);
		break;
	case L'r':
		res->type = TOK_REG;
		res->lexeme[0] = L'r';
		advance(state);
		size_t i = 1;
		wchar_t c = 0;
		while(iswdigit((c = la(state)))){
			if(i == MAX_LEXEME - 1){
				break;
			}
			res->lexeme[i++] = c;
			advance(state);
		}
		res->data = wcstoll(res->lexeme + 1, NULL, 10);
		break;
	CASEC(L':', TOK_COLON)
	CASEC(L',', TOK_COMMA)
	default:
		if(is_id_start(la(state))){
			res->type = TOK_ID;
			wchar_t c = 0;
			size_t index = 0;
			while(is_id_body((c = la(state)))){
				if(index == MAX_LEXEME - 2){
					break;
				}
				res->lexeme[index++] = c;
				advance(state);
			}
		}else{
			err("couldn't match character %lc", la(state));
		}
		break;
	};
	return 0;
}
