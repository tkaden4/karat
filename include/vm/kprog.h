#pragma once

/* karat files */
#include<ktypes.h>

#define KPROG_ENTRY_POINT L"__start"

struct kprog {
	u8 *program;
	size_t entry_point;
	size_t prog_size;
};

struct kprog *kprog_create();
void kprog_destroy(struct kprog *prog);
