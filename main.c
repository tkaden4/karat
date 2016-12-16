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
												L"sdl_keyboard");
	struct mod *screen = load_internal_module(	&modules, 
												(read_cb)RAW_CB(read), 
												L"sdl_screen");
	puts("loaded modules:");
	LIST_FOREACH(struct mod, modules.head, each){
		printf("\t%ls\n", each->name);
	}

	add_mmapped_io(&sys.ram, 0xFF80, keyboard->on_read);	
	add_mmapped_io(&sys.ram, 0xFF81, screen->on_read);	

	goto cleanup;
cleanup:
	mem_destroy(&sys.ram);
	modules_destroy(&modules);
	return 0;
}
