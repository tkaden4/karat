#include"system.h"

#include<string.h>
#include"log.h"

void system_init(struct system *sys)
{
	err_on(!sys, "attempt to initialize unallocated struct");
	mem_init(&sys->memory);
	cpu_init(&sys->cpu);
	memset(sys->ports, 0, IO_PORTS);
}

void set_port(struct system *sys, io_t which, struct mod *module)
{
	err_on(!sys, "uninitialized system");
	warn_on(which >= IO_PORTS, "port outside of range");
	if(which < IO_PORTS){
		warn_on(!module, "module in set_port not initialized");
		warn_on(sys->ports[which], "port already initialized, overwriting");
		sys->ports[which] = module;
	}
}

void reset_port(struct system *sys, io_t which)
{
	err_on(!sys, "uninitialized system");
	warn_on(which >= IO_PORTS, "port outside of range");
	if(which < IO_PORTS){
		sys->ports[which] = NULL;
	}
}

void system_destroy(struct system *sys)
{
	warn_on(!sys, "attempt to destroy unallocated struct");
	if(sys){
		mem_destroy(&sys->memory); 
	}
}
