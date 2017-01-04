#include"parse.h"

#include<wchar.h>

#include"mem.h"
#include"log.h"
#include"cpu.h"


static inline int find_op_str(const wchar_t *str, enum op_val *ret)
{
	err_on(!str, "str not allocated");
	for(size_t i = 0; i < MAX_OP; ++i){
		if(!wcscmp(str, op_strings[i])){
			*ret = i;
			return 0;
		}
	}
	return 1;
}

#define RMSPACE(wb) \
	while(*wb == L' ' || *wb == L'\t') ++wb;

#define ISASCII(n) ((char)(n) == (n))
#define ISNUM(n)	((n) >= L'0' && (n) <= L'9')

#define MEM_PREFIX 	L'$'
#define BIN_PREFIX 	L'%'
#define CONS_PREFIX L'#'
#define REG_PREFIX  L'r'

static inline int check_digits(const wchar_t *str)
{
	size_t amt = 0;
	while(ISASCII(*str) && ISNUM(*str)){
		++amt;
		++str;
	}
	if(!amt){
		return 0;
	}
	RMSPACE(str);
	if(*str){
		return 0;
	}
	/* exhausted all options */
	return 1;
}

static inline int is_register(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	if(str[0] != REG_PREFIX){
		return 0;
	}
	++str;
	/* if no next character, it cant be a register */
	if(!*str){
		return 0;
	}
	return check_digits(str);
}

static inline int is_memory(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	RMSPACE(str);
	if(*str != MEM_PREFIX){
		return 0;
	}

	++str;
	if(!*str){
		return 0;
	}
	return check_digits(str);
}

static inline int is_constant(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	if(*str != CONS_PREFIX){
		return 0;
	}
	++str;
	if(*str == BIN_PREFIX){
		++str;
	}
	return check_digits(str);
}

static inline wchar_t *get_arg(wchar_t *args)
{
	err_on(!args, "null to get_arg");
	RMSPACE(args);
	wchar_t *delim = wcschr(args, L',');
	if(!delim){
		return (args + wcslen(args));
	}
	return delim;
}

static inline enum op_mode get_arg_type(wchar_t *arg)
{
	err_on(!arg, "arg not allocated");
	RMSPACE(arg);
	if(is_register(arg)){
		return R;
	}
	if(is_memory(arg)){
		return A;
	}
	if(is_constant(arg)){
		return K;
	}
	warn("couldnt get type of %ls", arg);
	return N;
}

static inline int get_op_fmt(wchar_t *args, enum op_mode *ret)
{
	err_on(!args, "args not allocated");
	RMSPACE(args);
	if(!*args){
		debug("no arguments");
		*ret = N;
	}else{
		wchar_t *fend = get_arg(args);
		*fend = L'\0';
		enum op_mode first = get_arg_type(args);
		debug("first : %ls", args);
		wchar_t *send = get_arg(fend + 1);
		*send = L'\0';
		send = fend + 1;
		debug("second : %ls", send);
		/* TODO you left off here, Kaden */
		/* TODO check for no second arg */
		if(send == fend){
			*ret = first;
			return 0;
		}
		enum op_mode second = get_arg_type(send);
		*ret = combine_modes(first, second);
	}
	return 0;
}

int parse_line(wchar_t *buff, struct direc *ret)
{
	err_on(!buff, "buffer not allocated");
	err_on(!ret, "ret not allocated");

	RMSPACE(buff);

	if(!wcslen(buff)){
		return 1;
	}

	ret->is_ins = 1;

	wchar_t *sp = wcschr(buff, L' ');
	if(sp){
		*sp = L'\0';
	}

	/* determine the operator */
	{
		enum op_val op;
		if(find_op_str(buff, &op)){
			return 1;
		}
		ret->in.op = op;
	}

	/* determine operator format */
	{
		enum op_mode mode;
		++sp;
		RMSPACE(sp);
		if(get_op_fmt(sp, &mode)){
			return 1;
		}
		ret->in.mode = mode;
	}

	ret->in.args[0] = 0;
	ret->in.args[1] = 81;

	return 0;
}
