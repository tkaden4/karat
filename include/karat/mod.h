#pragma once

#include<karat/vm/opcode.h>
#include<karat/vm/vm.h>
#include<karat/vm/types.h>
#include<karat/list.h>

#if !__linux
#error "modules only supported on linux"
#endif

/* module description */
#define MODULE(name, author, version) \
    static const char *__mod_name = name; \
    static const char *__mod_author = author; \
    static const char *__mod_version = version

typedef void* lib_t;

typedef int(*module_init_f)(void **, struct load_data *);
typedef int(*module_unload_f)(void *);
typedef int(*module_on_trap_f)(void *, k8_t, struct vm *);

struct kmod {
    /* handle to library */
    lib_t handle;
    /* name of the module */
    const char * name;
    /* handler for trap instruction */
    module_on_trap_f on_trap;
    /* module's data */
    void *mod_data;
};

struct load_data;

int module_load(struct kmod *, const char *, struct load_data *);
int module_unload(struct kmod *);
