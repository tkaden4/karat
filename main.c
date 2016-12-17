#include<stdio.h>

#include"cpu.h"
#include"ram.h"
#include"module.h"

struct system {
	struct cpu_state cpu;
	struct mem ram;
};

void read(void *env, uint8_t data)
{
	printf("%p : 0x%x\n", env, data);
}

int main()
{
	struct modlist modules;
	modules_init(&modules);

	struct system sys;
	mem_init(&sys.ram);
	cpu_init(&sys.cpu);
	
	struct mod *keyboard = load_internal_module(&modules, 
												(read_cb)RAW_CB(read),
												0xFFFF,
												L"sdl_keyboard");

	struct mod *screen = load_internal_module(	&modules, 
												(read_cb)RAW_CB(read),
												0xFFFF,
												L"sdl_screen");

	add_mmapped_io(&sys.ram, keyboard->addr, keyboard->on_read);	
	add_mmapped_io(&sys.ram, screen->addr, screen->on_read);	

	puts("loaded modules:");
	LIST_FOREACH(struct mod, modules.head, each){
		printf("\t0x%4X : %ls\n", each->addr, each->name);
	}

	goto cleanup;
cleanup:
	mem_destroy(&sys.ram);
	modules_destroy(&modules);
	return 0;
}
