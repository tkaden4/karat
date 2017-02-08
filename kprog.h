#pragma once

#include"types.h"

struct kprog {
	size_t entry_point;
	u8 *program;
	size_t prog_size;
};

struct kprog *kprog_create();
void kprog_destroy(struct kprog *prog);
