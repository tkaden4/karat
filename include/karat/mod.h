#pragma once

#include<karat/vm/opcode.h>
#include<karat/vm/vm.h>
#include<karat/vm/types.h>
#include<karat/list.h>

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

struct int_list {
    SLINK(struct int_list);
    int code;
};

struct kmod {
    /* handle to library */
    lib_t handle;
    /* name of the module */
    const char * name;
    /* handler for trap instruction */
    module_on_trap_f on_trap;
    /* list of pending interrupts */
    struct int_list *pending;
    /* module's data */
    void *mod_data;
};

int module_load(struct kmod *, const char *);
int module_unload(struct kmod *);
