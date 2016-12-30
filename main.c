#include<locale.h>
#include<math.h>
#include<time.h>

#include"system.h"
#include"module.h"
#include"screen.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	if(argc == 2){
		puts(argv[1]);
	}else{
		struct system s;
		system_init(&s);
		system_load_builtins(&s);

		SDL_Delay(800);
		system_destroy(&s);
	}
	return 0;
}
