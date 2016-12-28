#include<stdio.h>
#include<math.h>

#include"log.h"
#include"system.h"
#include"module.h"
#include"screen.h"

void on_clear(void *data, u8 *args, size_t nargs)
{
	(void) data;
	printf("args [ ");
	for(size_t i = 0; i < nargs; ++i){
		printf("0x%X ", args[i]);
	}
	printf("]\n");
}

int main()
{
	struct system sys;
	system_init(&sys);

	struct mod scrmod;
	system_set_port(&sys, 0, screen_module(&scrmod));

	debug("0x%X", module_read(&scrmod));
	module_write(&scrmod, 0x80);

	SDL_Delay(850ull);
	system_destroy(&sys);
	return 0;
}
