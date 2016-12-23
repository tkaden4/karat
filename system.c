#include"system.h"

#include<string.h>

#include"mem.h"
#include"log.h"

void system_init(struct system *sys)
{
	err_on(!sys, "attempt to initialize unallocated struct");
	mem_init(&sys->memory);
	cpu_init(&sys->cpu);
	memset(sys->ports, 0, sizeof(struct mod *) * IO_PORTS);
}

void system_set_port(struct system *sys, io_t which, struct mod *module)
{
	err_on(!sys, "uninitialized system");
	warn_on(which >= IO_PORTS, "port outside of range");
	if(which < IO_PORTS){
		warn_on(!module, "module in set_port not initialized");
		warn_on(sys->ports[which], "port %d already initialized to \"%ls\"", 
				which, sys->ports[which]->name);
		module_init(module);
		sys->ports[which] = module;
	}
}

void system_reset_port(struct system *sys, io_t which)
{
	err_on(!sys, "uninitialized system");
	warn_on(which >= IO_PORTS, "port outside of range");
	if(which < IO_PORTS){
		if(sys->ports[which]){
			module_destroy(sys->ports[which]);
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
			}
		}
	}
}
