#pragma once
/* stdlib files */
#include<stdint.h>
#include<stdio.h>
/* karat files */
#include<karat/vm/kprog.h>
#include<karat/ktypes.h>

typedef uint32_t reg_t; /* general register datatype */

#define GENERAL_REGS 32
#define BLANK_CPU (struct cpu){ .regs = {0} }

struct cpu {
    /* general registers */
    union {
        reg_t regs[GENERAL_REGS];
        struct {
            reg_t __pad[GENERAL_REGS - 3];
            reg_t sp;
            reg_t pc;
        };
    };
};

static inline void cpu_init(struct cpu *cpu, reg_t pc, reg_t sp)
{
    cpu->sp = sp;
    cpu->pc = pc;
}
