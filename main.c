#include<locale.h>
#include<SDL2/SDL.h>

#include"log.h"
#include"system.h"
#include"opcode.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

/* turn the screen blue */
static u8 commands[] = { 
	0x81,				/* start flood */
		0, 0, 0xff,  	/* use blue */
	1					/* draw */ 
};
void do_commands(struct system *s, u8 port)
{
	for(register size_t i = 0; i < sizeof(commands); ++i){
		system_io_write(s, port, commands[i]);
	}
}

int main()
{
	setlocale(LC_ALL, "");
	
	struct system sys;
	system_init(&sys);
	system_load_builtins(&sys);

	do_commands(&sys, 0);

	SDL_Delay(800);

	system_destroy(&sys);
	return 0;
}
