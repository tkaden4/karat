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

	struct mod scrmod = BLANK_MOD;
	system_set_port(&sys, 0, screen_module(&scrmod));

	SDL_Delay(850ull);
	
	system_destroy(&sys);
	return 0;
}
