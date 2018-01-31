#include<stdio.h>
#include<stdint.h>
#include<string.h>

#include<dlfcn.h>

#include<karat/mod.h>
#include<karat/log.h>

#ifdef __linux
#define MODULE_FEXT ".so"
#endif

static inline lib_t __get_module_handle(const char *path)
{
    /* open the module */
    lib_t *handle = dlopen(path, RTLD_LAZY);
    err_on(!handle, "unable to open module: %s", dlerror());
    return handle;
}

static inline void *__get_module_symbol(lib_t handle, const char *sym_name)
{
    /* initialize the module */
    void *symbol = dlsym(handle, sym_name);
    return symbol;
}

static inline int __unload_module_handle(lib_t handle)
{
    err_on(dlclose(handle) == -1, "module unload: %s", dlerror());
    return 0;
}

/* TODO seperate module initialization from loading */
int module_load(struct kmod *mod, const char *name)
{
    ncheck(mod);
    ncheck(name);
    /* Create the new string representing the full path */
    size_t nlen = strlen(name);
    char path[nlen + sizeof(MODULE_FEXT) + 1];
    strcpy(path, name);
    strcpy(path + nlen, MODULE_FEXT);
    /* load the module */
    mod->handle = __get_module_handle(path);
    module_init_f init = __get_module_symbol(mod->handle, "on_module_load");
    err_on(!init, "module init not available for %s", name);
    int err = init(&mod->mod_data);
    if(err){
        return err;
    }
    module_write_f writer = __get_module_symbol(mod->handle, "on_port_write");
    err_on(!writer, "module writer not available for %s", name);
    mod->on_port_write = writer;
    return 0;
}

int module_unload(struct kmod *mod)
{
    ncheck(mod);
    module_unload_f unload = __get_module_symbol(mod->handle, "on_module_unload");
    err_on(!unload, "module unload not available");
    int err = unload(mod->mod_data);
    if(__unload_module_handle(mod->handle)){
        err = 1;
    }
    return err;
}
