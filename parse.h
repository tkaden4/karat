#pragma once

#include"system.h"
#include"cpu.h"
#include"opcode.h"

enum ind_mode {
	IND_RK,	/* register value plus number */
	IND_KK	/* value plus constant */
};

struct opcode {
	u8 code;
	u32 args[2];
};

struct ins {
	enum op_val op;
	enum op_mode mode;
	struct ins_arg {
		int is_indirect;
		enum ind_mode ind_mode;
		u32 arg[2];
	} args[2];;
};

struct direc {
	int is_ins;
	union {
		struct ins ins;
		wchar_t label[10];
	};
};

void parse_line(const wchar_t *buff, struct direc *ret);
void process_args(const struct cpu_state *cpu, 
				 const struct system *sys,
				 size_t nargs,
				 const struct ins_arg args[nargs], 
				 u32 res[nargs]);
