#pragma once
/* stdlib files */
#include<stdint.h>
#include<stdio.h>
/* karat files */
#include<vm/kprog.h>
#include<traits.h>
#include<ktypes.h>

typedef uint32_t reg_t;	/* general register datatype */

#define GENERAL_REGS 32

struct cpu {
	/* general registers */
	union {
		reg_t regs[GENERAL_REGS];
	};
	addr_t pc;
};

void cpu_init(struct cpu *state);
void cpu_reset(struct cpu *state);
void cpu_step(struct cpu *state, struct kprog *prog);
