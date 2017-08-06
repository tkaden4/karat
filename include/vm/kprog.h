#pragma once

/* karat files */
#include<ktypes.h>

#define KPROG_ENTRY_POINT L"__start"

struct kprog {
	i32 entry_point;
	size_t prog_size;
	/* vector */
	u8 *program;
	size_t __size;
	size_t __cap;
};

#define kprog_text(header) (((struct kprog *)(&header))->program + (header).text)
#define kprog_data(header) (((struct kprog *)(&header))->program + (header).data)
#define kprog_stack(header) (((struct kprog *)(&header))->program + (header).stack)

struct kprog *kprog_create();
/* append bytes to buffer */
void kprog_append_bytes(struct kprog *prog, u32 data, size_t bytes);
/* no longer able to append. This frees extra memory */
void kprog_finalize(struct kprog *prog);
void kprog_destroy(struct kprog *prog);
