#pragma once

/* karat files */
#include<ktypes.h>

/* PROGRAM FORMAT
 DATA SECTION
 ... (static data)
 CODE SECTION
 ... (bytecode)
 STACK SECTION
 ... (runtime data)
 */

/* TODO implement magic */

#define KPROG_ENTRY_POINT L"__start"
#define PROG_MAGIC ((u32)0x12344321)

struct prog_header {
    /* magic number */
    u32 magic;
    /* sections */
    u16 data_start;
    u16 text_start;
    u16 stack_start;
};

struct kprog {
    struct prog_header header;
	i32 entry_point;
	size_t prog_size;
	/* vector */
	u8 *program;
	size_t __size;
	size_t __cap;
};

const wchar_t *kprog_desc(const struct prog_header *, const struct kprog *);
u8 *kprog_data(const struct prog_header *, struct kprog *);
u8 *kprog_text(const struct prog_header *, struct kprog *);
u8 *kprog_stack(const struct prog_header *, struct kprog *);

struct kprog *kprog_create();
/* append bytes to buffer */
void kprog_append_bytes(struct kprog *prog, u32 data, size_t bytes);
/* no longer able to append. This frees extra memory */
void kprog_finalize(struct kprog *prog);
void kprog_destroy(struct kprog *prog);
