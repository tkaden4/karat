#pragma once

#include<karat/vm/cpu.h>
#include<karat/vm/types.h>
#include<karat/mod.h>

#include<stdint.h>

#define MAX_MODS UINT8_MAX

#define vm_opts(m) ((struct vm_options){ .memory_size = m })
#define vm_reg(vm, r) ((vm)->cpu.regs[r])

struct vm_options {
    uint32_t memory_size;
};

/* Arbitrarily-sized pages would sacrifice too much speed,
 * so we allocate 128*4 byte pages that have dynamic lookup
 * for setting and getting of information. Might be improved by
 * the introduction of either I/O ports or dedicated memory ranges
 * (i.e. above 0xffff0000) */
#define PAGE_QWORDS 128
#define PAGE_SIZE (PAGE_QWORDS*4)
#define NPAGES 1024

struct page {
    int(*write)(uint8_t, k32_t);
    int(*read)(uint8_t, k32_t *);
    void *data;
};

struct vm {
    struct kmod *mods[MAX_MODS];
    struct cpu cpu;
    uint8_t *memory;
    struct page *pages;
};

void vm_run(struct vm *vm, struct vm_options opts, const struct prog_t *prog);
