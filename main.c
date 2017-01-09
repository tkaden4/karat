#include<locale.h>
#include<SDL2/SDL.h>

#include"log.h"
#include"system.h"
#include"util.h"
#include"parse.h"
#include"opcode.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

#define TEST_FILE "test/asm.kt"

int main()
{
	setlocale(LC_ALL, "");

	FILE *test = fopen(TEST_FILE, "r");
	err_on(!test, "could not open %s", TEST_FILE);

	struct cpu_state cpu;
	struct system sys;
	cpu_init(&cpu);
	system_init(&sys);
	system_load_builtins(&sys);

	wchar_t buff[80];
	while(fgetws(buff, 80, test)){
		rmrn(buff);
		if(!wcslen(buff)){
			continue;
		}

		struct direc next;
		parse_line(buff, &next);
		
		//debug("%u %u", next.ins.args[0].arg[0], next.ins.args[0].arg[1]);

		if(!(valid_opmodes[next.ins.op] & next.ins.mode)){
			err("invalid opmode for %ls", op_strings[next.ins.op]);
		}
	}

	SDL_Delay(800);
	system_destroy(&sys);
	fclose(test);
	return 0;
}
