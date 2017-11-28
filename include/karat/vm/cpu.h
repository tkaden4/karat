#pragma once
/* stdlib files */
#include<stdint.h>
#include<stdio.h>
/* karat files */
#include<karat/vm/kprog.h>
#include<karat/ktypes.h>
#include<karat/log.h>

typedef uint32_t reg_t; /* general register datatype */

#define GENERAL_REGS 32
#define BLANK_CPU (struct cpu){ .regs = {0} }

struct cpu {
    /* general registers */
    union {
        reg_t regs[GENERAL_REGS];
        struct {
            reg_t __pad[GENERAL_REGS - 2];
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

static void print_cpu_info(struct cpu *cpu)
{
    err_on(!cpu, "cpu is NULL");
    for(size_t i = 0; i < GENERAL_REGS; ++i){
        printf("r%lu: %u\n", i, cpu->regs[i]);
    }
}
