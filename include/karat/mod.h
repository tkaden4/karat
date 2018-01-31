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

#define PORTS(n) \
    const int __mod_n_ports = n

typedef void* lib_t;

typedef int(*module_init_f)(void **);
typedef int(*module_unload_f)(void *);
typedef int(*module_write_f)(void *, unsigned, k32_t);

struct kmod {
    lib_t handle;
    const char * name;
    module_write_f on_port_write;
    /* data reserved for the module */
    void *mod_data;
};

int module_load(struct kmod *, const char *);
int module_unload(struct kmod *);
