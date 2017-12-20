#pragma once

#include<karat/vm/cpu.h>
#include<karat/vm/kprog.h>

#include<stdint.h>

struct vm_options {
    uint32_t memory_size;
};

#define vm_opts(m) ((struct vm_options){ .memory_size = m })

#define MAX_MODS UINT8_MAX

struct kmod;

struct vm {
    const struct kprog *prog;
    struct kmod *mods[MAX_MODS];
    struct cpu cpu;
    uint8_t *memory;
};

#define vm_reg(vm, r) ((vm)->cpu.regs[r])

void vm_run(struct vm *vm, struct vm_options opts, const struct kprog *prog);
