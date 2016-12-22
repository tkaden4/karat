#include<stdio.h>

#include"system.h"
#include"screen.h"

int main()
{
	struct screen_state scr;
	screen_init(&scr);
	screen_clear(&scr);

	screen_set_pixel(&scr, 0, 0, rgb(138, 43, 226));

	screen_draw(&scr);
	SDL_Delay(850);
	screen_destroy(&scr);
	return 0;
}
