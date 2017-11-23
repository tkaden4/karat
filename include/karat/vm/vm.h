#pragma once

#include<karat/ktypes.h>
#include<karat/vm/cpu.h>
#include<karat/vm/kprog.h>

struct vm_options {
    u32 memory_size;
};

struct vm {
    const struct kprog *prog;
    struct cpu cpu;
    u8 *memory;
};

void vm_run(struct vm *vm, struct vm_options opts, const struct kprog *prog);
