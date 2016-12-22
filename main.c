#include<stdio.h>
#include<math.h>

#include"log.h"
#include"system.h"
#include"module.h"
#include"screen.h"

int main()
{
	struct system sys;
	system_init(&sys);

	struct screen_state scr;
	screen_init(&scr);

	struct mod screen;
	screen_as_module(&scr, &screen);

	system_set_port(&sys, 0, &screen);

	SDL_Delay(850);

	module_destroy(&screen);
	system_destroy(&sys);
	return 0;
}
