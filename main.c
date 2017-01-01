#include<locale.h>

#include"system.h"
#include"module.h"
#include"screen.h"
#include"log.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

static u8 commands[] = { 0x81, 0, 0, 0xff, 1 };
void do_commands(struct system *s, u8 port)
{
	for(size_t i = 0; i < sizeof(commands); ++i){
		system_io_write(s, port, commands[i]);
	}
}

int main()
{
	setlocale(LC_ALL, "");

	struct system s;
	system_init(&s);
	system_load_builtins(&s);

	do_commands(&s, 0);
	SDL_Delay(800);

	system_destroy(&s);
	return 0;
}
