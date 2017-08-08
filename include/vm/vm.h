#pragma once

#include<ktypes.h>
#include<vm/cpu.h>
#include<vm/kprog.h>

struct vm {
    struct kprog *prog;
    struct cpu cpu;
    u8 *memory;
};

void vm_run(struct vm *vm, struct kprog *prog);
