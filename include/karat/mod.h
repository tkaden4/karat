#pragma once

#include<karat/vm/vm.h>
#include<karat/vm/types.h>
#include<karat/smap.h>

#if !__linux
#error "modules only supported on linux"
#endif

#define MODULE(name, author, version) \
    const char *__mod_name = name; \
    const char *__mod_author = author; \
    const char *__mod_version = version

typedef void* lib_t;

typedef int(*module_init_f)(void **);
typedef int(*module_unload_f)(void *);

struct kmod {
    lib_t handle;
    const char * name;
    /* data reserved for the module */
    void *mod_data;
};

int module_load(struct kmod *, const char *);
int module_unload(struct kmod *);
