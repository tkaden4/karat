#pragma once

#include<stdint.h>
#include<stdio.h>
#include<string.h>

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
            /* padding */
            reg_t __pad[GENERAL_REGS];
            /* aliased to regs[30] and regs[31] */
            reg_t sp;
            reg_t pc;
        };
    };
};

static inline void cpu_init(struct cpu *cpu, reg_t pc, reg_t sp)
{
    ncheck(cpu);
    memset(cpu, 0, sizeof(struct cpu));
    cpu->sp = sp;
    cpu->pc = pc;
}

static void print_cpu_info(struct cpu *cpu)
{
    ncheck(cpu);
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
