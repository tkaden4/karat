#include<locale.h>
#include<time.h>

#include"system.h"
#include"module.h"
#include"screen.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

int main()
{
	setlocale(LC_ALL, "");
	struct system s;
	system_init(&s);
	system_load_builtins(&s);

	/* start the draw_rgb command */
	system_io_write(&s, 0, 0x83);
	/* write the x and y position */
	system_io_write(&s, 0, 0x12);
	system_io_write(&s, 0, 0x12);
	/* write r, g, and b */
	system_io_write(&s, 0, 0);
	system_io_write(&s, 0, 0);
	system_io_write(&s, 0, 0xFF);
	/* draw the screen */
	system_io_write(&s, 0, 0x01);

	SDL_Delay(800);

	system_destroy(&s);
	return 0;
}
