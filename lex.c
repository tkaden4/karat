#include<wctype.h>
#include<ctype.h>
#include<string.h>

#include"alloc.h"
#include"lex.h"
#include"log.h"

static inline void add_until_f(
	const wchar_t **buff, wchar_t *to, size_t max, int(*valid)(wint_t))
{
	register size_t i = 0;
	for(; i < max - 1; ++i){
		if(valid(**buff)){
			to[i] = **buff;
			++*buff;
		}
	}
	to[i] = L'\0';
}

static int is_id_body(wint_t c)
{
	return iswalpha(c) || c == L'_';
}

static inline int lex_number(const wchar_t **line, struct token *res, size_t base)
{
	wchar_t *end = NULL;
	long long val = wcstoll(*line, &end, base);
	if(!end || end == *line){
		return 1;
	}
	wchar_t *crawl = res->lexeme;
	while(*line != end){
		*crawl++ = **line;
		++*line;
	}
	res->data = val;
	return 0;
}

static int next_token(const wchar_t **line, struct token *res)
{
	err_on(!res, "Token not allocated");
	err_on(!line || !*line, "Line not allocated");
	while(iswspace(**line)){
		++*line;
	}

	if(**line == L';'){
		while(**line && **line != L'\n'){
			++*line;
		}
		++*line;
		return 1;
	}

	memset(res->lexeme, 0, sizeof(wchar_t) * MAX_LEXEME);
	res->type = -1;
	#define CTOK(l, t) \
		++*line; \
		res->type = t; \
		res->lexeme[0] = l; \
		res->lexeme[1] = L'\0'; \
		break;

	switch(**line){
	case L'\0': return 1;
	case L'#': 
	{
		res->type = TOK_NUM;
		const wchar_t la = *++*line;
		size_t base = 0;
		if(la == L'$'){
			base = 16;
			++*line;
		}else if(la == L'%'){
			base = 2;
			++*line;
		}else if(iswdigit(la)){
			base = 10;
		}else{
			return 1;
		}
		return lex_number(line, res, base);
	}
	case L'$': 
		++*line;
		if(lex_number(line, res, 16)){
			return 1;
		}else{
			res->type = TOK_ADDR;
		}
		break;
	case L':': CTOK(L':', TOK_COLON);
	case L',': CTOK(L',', TOK_COMMA);
	case L'r':	/* register */
		++*line;
		if(!iswdigit(**line)){
			return 1;
		}
		res->lexeme[0] = L'r';
		res->type = TOK_REG;
		if(lex_number(line, res, 10)){
			return 1;
		}
		break;
	default:
		if(is_id_body(**line)){	/* ID */
			res->type = TOK_ID;
			add_until_f(line, res->lexeme, MAX_LEXEME, is_id_body); 
		}else if(iswdigit(**line)){
			res->type = TOK_NUM;
			add_until_f(line, res->lexeme, MAX_LEXEME, iswdigit); 
		}else{
			return 1;
		}
	};
	#undef CTOK
	return 0;
}

int tokenize_line(const wchar_t *line, struct token **toks, size_t *res_size)
{
	#define MAX_TOKS 10
	struct token ret[MAX_TOKS];
	size_t i = 0;
	while(!next_token(&line, &ret[i])){
		++i;
		if(i >= MAX_TOKS){
			return 1;
		}
	}
	if(!i){	/* it is ok if a line has no tokens */
		return 0;
	}
	*toks = s_calloc(i, sizeof(struct token));
	memcpy(*toks, ret, sizeof(struct token) * i);
	*res_size = i;
	return 0;
}
