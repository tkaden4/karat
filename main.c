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

	SDL_Delay(800);

	system_destroy(&s);
	return 0;
}
