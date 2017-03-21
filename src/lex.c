#include<wctype.h>
#include<ctype.h>
#include<string.h>
#include<parse/lex.h>
#include<parse/rbuff.h>
#include<alloc.h>
#include<log.h>

RBUFF_IMPL(lex_la_buff, wchar_t, MAX_LEX_LOOK)

static void lex_advance(struct lex_state *);
static wchar_t la(struct lex_state *);

static inline wchar_t readwc(FILE *f)
{
	wint_t c = fgetwc(f);
	return c == WEOF ? L'\0' : (wchar_t)c;
}

void lex_init(struct lex_state *state, FILE *f)
{
	state->file = f;
	state->line_no = 1;
	state->col_no = 1;
	lex_la_buff_init(&state->la_chars);
	for(size_t i = 0; i < MAX_LEX_LOOK; ++i){
		*lex_la_buff_push_back(&state->la_chars) = readwc(state->file);
	}
}

/* @deprecated */
void lex_destroy(struct lex_state *state)
{
	(void) state;
}

static wchar_t la(struct lex_state *state)
{
	return *lex_la_buff_front(&state->la_chars);
}

static void lex_advance(struct lex_state *state)
{
	wchar_t popped = *lex_la_buff_pop_front(&state->la_chars);
	++state->col_no;
	if(popped == L'\n'){
		++state->line_no;
		state->col_no = 1;
	}
	*lex_la_buff_push_back(&state->la_chars) = readwc(state->file);
}

static inline int is_id_start(wint_t c)
{
	return iswalpha(c) || c == L'_';
}

static inline int is_id_body(wint_t c)
{
	return iswalpha(c) || iswdigit(c) || c == '_';
}

static inline int iswbin(wint_t c)
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
		lex_advance(state);
	}
	res->data = wcstoll(res->lexeme, NULL, base);
	return 0;
}


int lex_next(struct lex_state *state, struct token *res)
{
	err_on(!res, "Token not allocated");
	while(la(state) == L'\t' || la(state) == L' '){
		lex_advance(state);
	}
	if(la(state) == L';'){
		while(la(state) && la(state) != L'\n'){
			lex_advance(state);
		}
	}
	memset(res, 0, sizeof(struct token));

	#define CASEC(c, t)\
		case c: res->lexeme[0] = c; res->type = t; lex_advance(state); break;

	int err = 0;
	switch(la(state)){
	case L'\n':
		res->type = TOK_EOL;
		wcsncpy(res->lexeme, L"EOL", MAX_LEXEME);
		lex_advance(state);
		break;
	case L'\0':
		res->type = TOK_EOS;
		wcsncpy(res->lexeme, L"EOS", MAX_LEXEME);
		break;
	case L'.':
		lex_advance(state);
		if(iswalpha(la(state))){
			res->type = TOK_DOT_CHAR;
			res->lexeme[0] = L'.';
			res->lexeme[1] = la(state);
			lex_advance(state);
			break;
		}else{
			err = 1;
		}
		break;
	case L'#':
		res->type = TOK_NUM;
		lex_advance(state);
		switch(la(state)){
		case L'$': lex_advance(state); lex_number(state, res, 16); break;
		case L'%': lex_advance(state); lex_number(state, res, 2); break;
		default:
			if(iswdigit(la(state))){
				lex_number(state, res, 10);
			}else{
				err = 1;
			}
		};
		break;
	case L'$':
		res->type = TOK_ADDR;
		lex_advance(state);
		lex_number(state, res, 16);
		break;
	case L'r':
		res->type = TOK_REG;
		res->lexeme[0] = L'r';
		lex_advance(state);
		size_t i = 1;
		wchar_t c = 0;
		while(iswdigit((c = la(state)))){
			if(i == MAX_LEXEME - 1){
				break;
			}
			res->lexeme[i++] = c;
			lex_advance(state);
		}
		res->data = wcstoll(res->lexeme + 1, NULL, 10);
        if(res->data > 31 || res->data < 0){
            fprintf(stderr, "no register r%lld\n", res->data);
            exit(0);
        }
		break;
	CASEC(L':', TOK_COLON)
	CASEC(L',', TOK_COMMA)
	default:
		if(is_id_start(la(state))){
			res->type = TOK_ID;
			wchar_t c = 0;
			size_t index = 0;
			while(is_id_body((c = la(state)))){
				if(index >= MAX_LEXEME - 2){
					break;
				}
				res->lexeme[index++] = c;
				lex_advance(state);
			}
		}else{
			err("couldn't match character %lc", la(state));
		}
		break;
	};

	res->line_no = state->line_no;
	res->col_no = state->col_no;

	return err;
}
