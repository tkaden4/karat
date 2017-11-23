#pragma once

#include<karat/ktypes.h>
#include<karat/vm/cpu.h>
#include<karat/vm/kprog.h>

struct vm_options {
    uint32_t memory_size;
};

struct vm {
    const struct kprog *prog;
    struct cpu cpu;
    uint8_t *memory;
};

void vm_run(struct vm *vm, struct vm_options opts, const struct kprog *prog);
