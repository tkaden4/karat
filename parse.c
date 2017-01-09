#include"parse.h"

#include<stdio.h>
#include<ctype.h>
#include<wchar.h>

#include"mem.h"
#include"log.h"
#include"cpu.h"

#define min(a, b) ((a) > (b)) ? (b) : (a)

/* error handling */
enum perr {
	ARG_FMT,
	OP_FMT,
	MAX_PERR,
};

static const wchar_t *const parse_errs[MAX_PERR] = {
	L"arguments incorrectly formatted",
	L"mnemonic incorrectly formatted",
};

/* line number */
static size_t line_no = 1;
/* line itself */
static const wchar_t *err_line = L"";

#define FATAL_ERR(e) \
	printf("Fatal on line %lu \"%ls\": %ls\n", line_no, err_line, parse_errs[e]);\
	exit(1);

#define TRY(exp) do { \
	int e = exp; \
	if(e){ FATAL_ERR(e); } \
}while(0);

/* set a single argument to its final value */
static inline void process_arg(	const struct cpu_state *cpu,
								const struct system *sys, 
								const struct ins_arg *arg,
								u32 *res)
{
	err_on(!cpu, "cpu not allocated");
	err_on(!sys, "sys not allocated");
	err_on(!arg, "arg not allocated");
	err_on(!res, "res not allocated");

	if(!arg->is_indirect){
		*res = arg->arg[0];
		return;
	}

	switch(arg->ind_mode){
	case IND_KK:
		debug("constant + constant indirection");
		*res = (arg->arg[0] + arg->arg[1]);
		break;
	case IND_RK:
		debug("register + constant indirection");
		*res = (cpu->regs[arg->arg[0]] + arg->arg[1]);
		break;
	};
}

/* get the values of two arguments */
void process_args( 	const struct cpu_state *cpu, 
					const struct system *sys, 
					size_t nargs,
					const struct ins_arg args[nargs], 
					u32 res[nargs])
{
	err_on(!cpu, "cpu not allocated");
	err_on(!sys, "sys not allocated");
	err_on(!args, "arg not allocated");
	err_on(!res, "res not allocated");

	u32 new_args[nargs];

	for(size_t i = 0; i < nargs; ++i){
		process_arg(cpu, sys, &args[i], &new_args[i]);
	}
	memcpy(res, new_args, sizeof(u32)*nargs);
}

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

#define MEM_PREFIX 	L"$"
#define BIN_PREFIX 	L"%"
#define HEX_PREFIX L"$"
#define CONS_PREFIX L"#"
#define REG_PREFIX  L"r"

static inline int check_digits(const wchar_t *str)
{
	size_t amt = 0;
	while(ISASCII(*str) && ISNUM(*str)){
		++amt;
		++str;
	}
	/* check if found zero characters */
	if(!amt){
		return 0;
	}
	/* check if at end of string */
	RMSPACE(str);
	if(*str){
		return 0;
	}
	/* exhausted all options */
	return 1;
}

static inline int starts_with(const wchar_t *first, const wchar_t *second)
{
	err_on(!first, "first not allocated");
	err_on(!second, "second not allocated");
	size_t first_len = wcslen(first);
	size_t second_len = wcslen(second);
	if(first_len >= second_len){
		return !wcsncmp(first, second, second_len);
	}
	return 0;
}

static inline int is_argument(	const wchar_t *str, 
								const wchar_t *prefix, 
								int(*end_test)(const wchar_t *))
{
	err_on(!str, "str not allocated");
	err_on(!prefix, "prefix not allocated");
	int has_prefix = starts_with(str, prefix);
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

static inline int is_constant(const wchar_t *str)
{
	err_on(!str, "str not allocated");
	int hex = is_argument(str, CONS_PREFIX HEX_PREFIX, check_digits);
	int bin = is_argument(str, CONS_PREFIX BIN_PREFIX, check_digits);
	int dec = is_argument(str, CONS_PREFIX, check_digits);
	return hex || bin || dec;
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

static inline void get_arg_type(wchar_t *arg, enum op_mode *res)
{
	err_on(!arg, "arg not allocated");
	RMSPACE(arg);
	if(is_register(arg)){
		*res = R;
	}else if(is_memory(arg)){
		*res = A;
	}else if(is_constant(arg)){
		*res = K;
	}else{
		FATAL_ERR(ARG_FMT);
	}
}

/* convert argument string to value */
static inline void get_arg_value(const wchar_t *arg, enum op_mode m, u32 rarg[2])
{
	RMSPACE(arg);
	switch(m){
	case R:
		{
			wchar_t *str;
			u32 parsed = wcstol(arg + wcslen(REG_PREFIX), &str, 0);
			rarg[0] = parsed;
		}
		break;
	case A:
		warn("address argument not supported");
		break;
	case K:
		{
			wchar_t *str;
			u32 parsed = wcstol(arg + 1, &str, 0);
			rarg[0] = parsed;
		}
		break;
	default:
		err("value not handled");
	};
}

/* find the two argument strings in the opcode */
static inline void find_ops(wchar_t *args, wchar_t **one, wchar_t **two)
{
	wchar_t *first_end = get_arg(args);
	*first_end = L'\0';
	*one = args;

	args = first_end + 1;

	wchar_t *second_end = get_arg(args);
	*second_end = L'\0';
	*two = args;
}

static inline void parse_arguments(wchar_t *args, enum op_mode *ret, 
								  struct ins_arg rargs[2])
{
	err_on(!args, "args not allocated");
	RMSPACE(args);
	if(!*args){
		debug("no arguments");
		*ret = N;
	}else{
		wchar_t *first_arg;
		wchar_t *second_arg;
		find_ops(args, &first_arg, &second_arg);

		enum op_mode first = N;
		get_arg_type(first_arg, &first);

		enum op_mode second = N;
		get_arg_type(second_arg, &second);

		*ret = combine_modes(first, second);

		get_arg_value(first_arg, first, rargs[0].arg);
		get_arg_value(second_arg, second, rargs[1].arg);

	}
}

void parse_instruction(wchar_t *buff, struct ins *ins)
{
	err_on(!buff, "buffer not allocated");
	err_on(!ins, "ins not allocated");

	wchar_t *sp = wcschr(buff, L' ');

	if(sp){
		*sp = L'\0';
	}

	/* determine the operator */
	{
		enum op_val op;
		if(find_op_str(buff, &op)){
			err("couldnt find operator %ls", buff);
			return;
		}
		ins->op = op;
	}

	/* parse arguments into opcode */
	{
		enum op_mode mode;
		++sp;
		RMSPACE(sp);
		parse_arguments(sp, &mode, ins->args);
		ins->mode = mode;
	}
}

int is_label(const wchar_t *buff)
{
	err_on(!buff, "buffer not allocated");
	return 0;
}

void parse_line(const wchar_t *buff, struct direc *ret)
{
	err_on(!buff, "buffer not allocated");
	err_on(!ret, "ret not allocated");

	RMSPACE(buff);

	/* set line for error handling */
	err_line = buff;
	const size_t buff_len = wcslen(buff) + 1;

	if(!buff_len){
		++line_no;
		return;
	}

	/* create buffer for processing */
	wchar_t proc_buff[buff_len];
	memcpy(proc_buff, buff, sizeof(wchar_t) * buff_len);

	/* check if label */
	if(is_label(proc_buff)){
		ret->is_ins = 0;
		/* copy buffer into label */
		memcpy(ret->label, proc_buff, sizeof(wchar_t) * min(sizeof(ret->label), buff_len));
	}else{
		ret->is_ins = 1;
		/* otherwise, try as instruction */
		parse_instruction(proc_buff, &ret->ins);
	}
	
	/* increase line number */
	++line_no;
}
