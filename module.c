#include"module.h"

#include<stdio.h>
#include<wchar.h>

#include"mem.h"
#include"log.h"

struct mod * load_internal_module(struct mod *module, 
					read_cb on_read,
					write_cb on_write,
					destroy_cb on_deinit,
					const wchar_t *name)
{
	err_on(!module, "module not allocated");
	module->on_read = on_read;
	module->on_request = on_write;
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

void module_write(struct mod *module, uint8_t data)
{
	err_on(!module, "module not allocated");
	CALL_CB(module->on_read, data);
}

uint8_t module_read(struct mod *module)
{
	err_on(!module, "module not allocated");
	return CALL_CB(module->on_request);
}

void module_destroy(struct mod *module)
{
	warn_on(!module, "module not allocated");
	if(module){
		CALL_CB(module->on_destroy);
	}
}
