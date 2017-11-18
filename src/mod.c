#include<stdio.h>
#include<string.h>

#include<karat/mod.h>
#include<karat/log.h>
#include<dlfcn.h>

#ifdef __linux
#define MODULE_FEXT ".so"
#endif

static inline lib_t __get_module_handle(const char *path)
{
#ifdef __linux
    /* open the module */
    lib_t *handle = dlopen(path, RTLD_NOW);
    err_on(!handle, "%s", dlerror());
    return handle;
#endif
}

static inline void *__get_module_symbol(lib_t handle, const char *sym_name)
{
#ifdef __linux
    /* initialize the module */
    void *symbol = dlsym(handle, sym_name);
    err_on(!symbol, "loading symbol \"%s\" : %s", sym_name, dlerror());
    return symbol;
#endif
}

static inline int __unload_module_handle(lib_t handle)
{
#ifdef __linux
    err_on(dlclose(handle) == -1, "module unload: %s", dlerror());
    return 0;
#endif
}

int module_load(struct kmod *mod, const char *name)
{
    /* Create the new string representing the full path */
    size_t nlen = strlen(name);
    char path[nlen + sizeof(MODULE_FEXT) + 1];
    strcpy(path, name);
    strcpy(path + nlen, MODULE_FEXT);
    /* load the module */
    mod->handle = __get_module_handle(path);
    return 0;
}

/* XXX remove */
int module_unload(struct kmod *mod)
{
    return __unload_module_handle(mod->handle);
}
