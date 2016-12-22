#include<stdio.h>

#include"system.h"

void read(void *env, uint8_t data)
{
	printf("%p : 0x%x\n", env, data);
}

uint8_t write()
{
	return 0x80;
}

int main()
{
	struct system sys;
	system_init(&sys);

	/* add the keyboard module to the system */
	struct mod keyboard;
	set_port(&sys, 0, load_internal_module(
			 &keyboard, (read_cb)RAW_CB(read), (write_cb)RAW_CB(write), 
			 L"sdl_keyboard"));
	
	system_destroy(&sys);
	return 0;
}
