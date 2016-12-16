#pragma once

/* MODULE.H 
 *
 * Submodules may be added to the system 
 * to provide new functionality to the 
 * Karat system. Each module may use
 * io ports to communicate with the cpu.
 * code can use input to make decisions
 */

#include<stdint.h>
#include<wchar.h>

#include"karat.h"
#include"list.h"
#include"types.h"

struct mod {
	NODE(struct mod);
	/* name of the module */
	const wchar_t *name;
	/* what to do when cpu writes to port */
	read_cb on_read;
};

struct modlist {
	size_t size;
	struct mod *head;
	struct mod *tail;
};

/* initialize a module list with the maximum */
API void modules_init(struct modlist *list);
/* load a module from file */
API struct mod *load_module(struct modlist *list, const wchar_t *file);
/* create a raw module from callback (for builtin modules) */
API struct mod *load_internal_module(	struct modlist *list, 	
										read_cb update_cb, 
										const wchar_t *name	);
/* call the update method of a module with the given data */
API void update_module(struct mod *m, uint8_t data);
/* remove a module, if it does not exist, no effect */
API void unload_module(struct modlist *list, struct mod *m);
API void modules_destroy(struct modlist *list);
