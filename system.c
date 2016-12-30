#include"system.h"

#include<string.h>

#include"screen.h"
#include"mem.h"
#include"log.h"

void system_init(struct system *sys)
{
	err_on(!sys, "attempt to initialize unallocated struct");
	mem_init(&sys->memory);
	cpu_init(&sys->cpu);
	for(register size_t i = 0; i < IO_PORTS; ++i){
		sys->ports[i] = NULL;
	}
}

void system_load_builtins(struct system *sys)
{
	err_on(!sys, "uninitialized system");
	struct mod m;
	system_set_port(sys, 0, screen_module(&m));
}

void system_io_write(struct system *sys, io_t which, u8 data)
{
	err_on(!sys, "uninitialized system");
	err_on(which >= IO_PORTS, "port outside of range");
	warn_ret(!sys->ports[which], "port not initialized");
	module_write(sys->ports[which], data);
}

void system_set_port(struct system *sys, io_t which, struct mod *module)
{
	err_on(!sys, "uninitialized system");
	err_on(which >= IO_PORTS, "port outside of range");
	err_on(!module, "module in set_port not initialized");
	warn_on(sys->ports[which], "port %d already initialized to \"%ls\"", 
			which, sys->ports[which]->name);
	struct mod *new = s_alloc(struct mod);
	*new = *module;
	module_init(new);
	sys->ports[which] = new;
}

void system_reset_port(struct system *sys, io_t which)
{
	err_on(!sys, "uninitialized system");
	warn_on(which >= IO_PORTS, "port outside of range");
	if(which < IO_PORTS){
		if(sys->ports[which]){
			module_destroy(sys->ports[which]);
			s_free(sys->ports[which]);
		}
		sys->ports[which] = NULL;
	}
}

void system_destroy(struct system *sys)
{
	warn_on(!sys, "attempt to destroy unallocated struct");
	if(sys){
		mem_destroy(&sys->memory); 
		for(size_t i = 0; i < IO_PORTS; ++i){
			if(sys->ports[i]){
				module_destroy(sys->ports[i]);
				s_free(sys->ports[i]);
			}
		}
	}
}
