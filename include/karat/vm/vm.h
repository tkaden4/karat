#pragma once

#include<karat/ktypes.h>
#include<karat/vm/cpu.h>
#include<karat/vm/kprog.h>

struct vm {
    const struct kprog *prog;
    struct cpu cpu;
    u8 *memory;
};

void vm_run(struct vm *vm, const struct kprog *prog);
