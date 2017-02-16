#pragma once
/* stdlib files */
#include<stdint.h>
/* karat files */
#include<traits.h>
#include<ktypes.h>

typedef uint32_t reg_t;	/* general register datatype */

#define GENERAL_REGS 32

struct cpu {
	/* general registers */
	union {
		reg_t regs[GENERAL_REGS];
		reg_t __pad[GENERAL_REGS - 1];
		reg_t sp;
	};
	addr_t pc;
};

void cpu_init(struct cpu *state);
void cpu_reset(struct cpu *state);
