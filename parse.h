#pragma once

#include"opcode.h"

struct ins {
	enum op_val op;
	enum op_mode mode;
	u32 args[2];
};

struct direc {
	int is_ins;
	union {
		struct ins in;
		wchar_t label[10];
	};
};

int parse_line(wchar_t *buff, struct direc *ret);
