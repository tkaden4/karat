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
#include"types.h"

#define BLANK_MOD (struct mod){ L"", BLANK_CB, BLANK_CB, BLANK_CB, BLANK_CB }
#define MODULE_FTABLE(i, id, r, rd, w, wd, d, dd) \
	MAKE_CB(init_cb, i, id), \
	MAKE_CB(read_cb, r, rd), \
	MAKE_CB(write_cb, w, wd), \
	MAKE_CB(destroy_cb, d, dd) \

struct mod {
	/* name of the module */
	const wchar_t *name;
	/* how to initialize the module */
	init_cb on_init;
	/* what to do when cpu writes to port */
	read_cb on_read;
	/* what to do when cpu reads from port */
	write_cb on_request;
	/* what to do when done with module */
	destroy_cb on_destroy;
};

struct mod *load_internal_module(struct mod *module, 
					init_cb on_init,
					read_cb on_read,
					write_cb on_write,
					destroy_cb on_deinit,
					const wchar_t *name);
struct mod *load_external_module(struct mod *module, const wchar_t *file);

void module_init(struct mod *module);
void module_write(struct mod *module, uint8_t data);
uint8_t module_read(struct mod *module);
void module_destroy(struct mod *module);
