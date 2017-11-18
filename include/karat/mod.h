#pragma once

#include<karat/vm/opcode.h>

#if !__linux
#error "modules only supported on linux"
#endif

typedef void* lib_t;

struct kmod {
    lib_t handle;
    const char * name;
};

typedef int(*module_init_f)(struct kmod *);
typedef int(*module_unload_f)(struct kmod *);

int module_load(struct kmod *, const char *);
int module_unload(struct kmod *);
