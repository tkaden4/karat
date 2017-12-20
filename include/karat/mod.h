#pragma once

#include<karat/vm/opcode.h>
#include<karat/vm/vm.h>
#include<karat/vm/types.h>

#if !__linux
#error "modules only supported on linux"
#endif

#define MODULE(name, author, version) \
    const char *mod_name = name; \
    const char *mod_author = author; \
    const char *mod_version = version

typedef void* lib_t;

typedef int(*module_init_f)(void **);
typedef int(*module_unload_f)(void *);
typedef int(*module_on_trap_f)(void *, k8_t, struct vm *);

struct kmod {
    lib_t handle;
    const char * name;
    module_on_trap_f on_trap;
    void *mod_data;
};

int module_load(struct kmod *, const char *);
int module_unload(struct kmod *);
