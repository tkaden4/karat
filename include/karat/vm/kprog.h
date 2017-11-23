#pragma once

/* karat files */
#include<stdint.h>

#define KPROG_ENTRY_POINT L"__start"

struct kprog {
    int32_t entry_point;
    size_t prog_size;
    /* vector */
    uint8_t *program;
    size_t __size;
    size_t __cap;
};

struct kprog *kprog_create();
/* append bytes to buffer */
void kprog_append_bytes(struct kprog *prog, uint32_t data, size_t bytes);
/* no longer able to append. This frees extra memory */
void kprog_finalize(struct kprog *prog);
void kprog_destroy(struct kprog *prog);
