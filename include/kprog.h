#pragma once

#include"types.h"

#define KPROG_ENTRY_POINT L"__start"

struct kprog {
	u8 *entry_point;
	u8 *program;
	size_t prog_size;
};

struct kprog *kprog_create();
void kprog_destroy(struct kprog *prog);
