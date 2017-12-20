#pragma once
/* stdlib files */
#include<stdint.h>
#include<stdio.h>
/* karat files */
#include<karat/vm/kprog.h>
#include<karat/log.h>

#define ALL_REGS 32
#define GENERAL_REGS (ALL_REGS - 2)
#define BLANK_CPU (struct cpu){ .regs = {0} }

typedef uint32_t reg_t; /* general register datatype */

struct cpu {
    /* general registers */
    union {
        reg_t regs[ALL_REGS];
        struct {
            reg_t __pad[GENERAL_REGS];
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
    for(size_t i = 0; i < ALL_REGS; ++i){
        switch(i){
        case 30:
            printf("sp: %u\n", cpu->regs[i]);
            break;
        case 31:
            printf("pc: %u\n", cpu->regs[i]);
            break;
        default:
            printf("r%lu: %u\n", i, cpu->regs[i]);
            break;
        };
    }
}
