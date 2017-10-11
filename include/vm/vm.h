#pragma once

#include<ktypes.h>
#include<vm/cpu.h>
#include<vm/kprog.h>

struct vm {
    const struct kprog *prog;
    struct cpu cpu;
    u8 *memory;
};

void vm_run(struct vm *vm, const struct kprog *prog);
