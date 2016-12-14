#pragma once

/* MODULE.H 
 *
 * Submodules may be added to the system 
 * to provide new functionality to the 
 * Karat system. Each module may use 
 * up to 2 of the 16 predifined io ports
 */

#include<stdint.h>

#include"karat.h"

typedef void(*read_f)(void *, uint32_t);

struct mod {
	addr_t address;
	read_f on_read;
};

struct modlist {
};

API void modules_init()
/* load a module from file */
API void load_module(struct mod *m, const wchar_t *file);
/* create a raw module from function (for builtin modules) */
API void load_internal_module(struct mod *m, read_f sys_on_read);
/* remove a module */
API void unload_module(addr_t from_port);
