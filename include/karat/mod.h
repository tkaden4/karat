#pragma once

#include<karat/vm/opcode.h>

#if !__linux
#error "modules only supported on linux"
#endif

#define MODULE(name, author) \
    const char *mod_name = name; \
    const char *mod_author = author
    

typedef void* lib_t;

struct kmod {
    lib_t handle;
    const char * name;
    void *mod_data;
};

typedef int(*module_init_f)(void **);
typedef int(*module_unload_f)(void *);

int module_load(struct kmod *, const char *);
int module_unload(struct kmod *);
