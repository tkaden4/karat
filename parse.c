#include"parse.h"

#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>

#include"util.h"
#include"alloc.h"
#include"log.h"
#include"cpu.h"

#define min(a, b) ((a) > (b)) ? (b) : (a)

/* error handling */
enum perr {
	ARG_FMT = 0,
	OP_FMT,
	MAX_PERR,
};

static const wchar_t *const parse_errs[MAX_PERR] = {
	L"arguments incorrectly formatted",
	L"mnemonic incorrectly formatted",
};

static struct {
	/* line number */
	size_t line_no;
	/* line itself */
	const wchar_t *line;
} err_info = {1, L""};

#define parse_err(msg, ...) \
	fprintf(stderr, "Parse error : " msg "\n", ##__VA_ARGS__);

#define FATAL_ERR(e) \
	printf("Fatal on line %lu \"%ls\": %ls\n", \
		err_info.line_no, err_info.line, parse_errs[e]); \
	exit(1);

#define TRY(exp) do { \
	int e = exp; \
	if(e){ FATAL_ERR(e); } \
}while(0);


#define ISASCII(n) ((char)(n) == (n))
#define ISNUM(n)	((n) >= L'0' && (n) <= L'9')

#define MEM_PREFIX 	L"$"
#define BIN_PREFIX 	L"%"
#define HEX_PREFIX L"$"
#define CONS_PREFIX L"#"
#define REG_PREFIX  L"r"

static inline int check_digits(const wchar_t *str)
{
	/* make sure we have numbers to work with */
	{
		size_t amt = 0;
		while(iswdigit(*str)){
			++amt;
			++str;
		}
		if(!amt){
			return 0;
		}
	}

	/* check if at end of string */
	str = wcsig(str, L' ');
	if(*str){
		return 0;
	}
	/* exhausted all options */
	return 1;
}

static inline int is_argument(	const wchar_t *str, 
								const wchar_t *prefix, 
								int(*end_test)(const wchar_t *))
{
	err_on(!str, "str not allocated");
	err_on(!prefix, "prefix not allocated");
	const int has_prefix = starts_with(str, prefix);
	str += wcslen(prefix);
	return has_prefix ? end_test(str) : 0;
}

static inline int is_register(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	return is_argument(str, REG_PREFIX, check_digits);
}

static inline int is_memory(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	return is_argument(str, MEM_PREFIX, check_digits);
}

static inline int is_decimal(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	return is_argument(str, CONS_PREFIX, check_digits);
}

static inline int is_hex(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	return is_argument(str, CONS_PREFIX HEX_PREFIX, check_digits);
}

static inline int is_binary(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	return is_argument(str, CONS_PREFIX BIN_PREFIX, check_digits);
}

static inline int is_constant(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	return is_binary(str) || is_hex(str) || is_decimal(str);
}

int label_test(wint_t t)
{
	return iswalnum(t) || t == L'_';
}

int is_label(const wchar_t *buff)
{
	err_on(!buff, "buffer not allocated");
	const char test = ISASCII(*buff) ? (char)*buff : '\0';
	if(!label_test(test)){
		debug("not alpha numeric");
		return 0;
	}
	buff = wcsig_f(buff, label_test);
	if(*buff != L':'){
		return 0;
	}
	++buff;
	if(*wcsig(buff, L' ')){
		return 0;
	}
	return 1;
}

/* XXX todo fit to new opcode system */
int is_mnemonic(const wchar_t *buff)
{
	err_on(!buff, "buff not allocated");
	//return !find_op_str(buff, &tmp);
	return 0;
}

bool parse_line(const wchar_t *line, struct presult *res)
{
	(void) line;
	(void) res;
	++err_info.line_no;
	return true;
}
