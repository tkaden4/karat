#include"module.h"

#include<stdio.h>
#include<wchar.h>

#include"mem.h"
#include"log.h"

struct mod *load_internal_module(struct mod *module, 
					init_cb on_init,
					write_cb on_write,
					destroy_cb on_deinit,
					struct io_table table,
					const wchar_t *name)
{
	err_on(!module, "module not allocated");
	module->on_init = on_init;
	module->io = table;
	module->on_write = on_write;
	module->on_destroy = on_deinit;
	module->name = name;
	return module;
}

struct mod *load_external_module(struct mod *module, const wchar_t *file)
{
	err_on(!module, "module not allocated");
	printf("loading module from file \"%ls\"\n", file);
	return module;
}

void module_init(struct mod *module)
{
	err_on(!module, "module not allocated");
	CALL_CB(module->on_init);
}

void module_write(struct mod *module, uint8_t data)
{
	err_on(!module, "module not allocated");
	io_table_send(&module->io, data);
}

uint8_t module_read(struct mod *module)
{
	err_on(!module, "module not allocated");
	return CALL_CB(module->on_write);
}

void module_destroy(struct mod *module)
{
	warn_on(!module, "module not allocated");
	if(module){
		CALL_CB(module->on_destroy);
	}
	io_table_destroy(&module->io);
}
