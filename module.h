#pragma once

/* MODULE.H 
 *
 * Submodules may be added to the system 
 * to provide new functionality to the 
 * Karat system. Each module may use
 * io ports to communicate with the cpu.
 */

#include<stdint.h>
#include<wchar.h>

#include"karat.h"
#include"types.h"

#define MODULE_FTABLE(i, id, w, wd, d, dd) \
	MAKE_CB(init_cb, i, id), \
	MAKE_CB(write_cb, w, wd), \
	MAKE_CB(destroy_cb, d, dd) \

#define MAX_HANDLES 10

typedef CALLBACK(void, u8 *args, size_t nargs) io_cb;

struct io_command {
	/* ID */
	u8 code;
	/* how many arguments does it take? */
	size_t nargs;
	/* what to do when bytes are all read */
	io_cb command;
};

struct io_table {
	struct io_command handles[MAX_HANDLES];
	/* current io command we are trying to handle */
	struct io_command *current;
	/* how many arguments we have read */
	struct arg_vec {
		u8 *args;
		size_t size;
		size_t cap;
	} cmdargs;
};

void io_table_init(struct io_table *table);
void io_table_add(struct io_table *table, u8 code, size_t args, io_cb cb);
void io_table_send(struct io_table *table, u8 byte);
void io_table_destroy(struct io_table *table);

struct mod {
	/* name of the module */
	const wchar_t *name;
	/* how to initialize the module */
	init_cb on_init;
	/* io table for handling writes */
	struct io_table io;
	/* what to do when read from */
	write_cb on_write;
	/* what to do when done with module */
	destroy_cb on_destroy;
};

struct mod *load_internal_module(struct mod *module, 
					init_cb on_init,
					write_cb on_write,
					destroy_cb on_deinit,
					struct io_table table,
					const wchar_t *name);
struct mod *load_external_module(struct mod *module, const wchar_t *file);

void module_init(struct mod *module);
void module_write(struct mod *module, uint8_t data);
uint8_t module_read(struct mod *module);
void module_destroy(struct mod *module);
