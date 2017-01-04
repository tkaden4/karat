#include<locale.h>
#include<SDL2/SDL.h>

#include"log.h"
#include"system.h"
#include"util.h"
#include"parse.h"
#include"opcode.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

int main()
{
	setlocale(LC_ALL, "");

	FILE *test = fopen("test/asm.kt", "r");
	err_on(!test, "could not open test/asm.kt");

	wchar_t buff[80];
	while(fgetws(buff, 80, test)){
		rmrn(buff);
		struct direc next;
		err_on(parse_line(buff, &next), "unable to parse %ls", buff);
		debug("%ls", op_strings[next.in.op]);
		debug("%u %u", next.in.args[0], next.in.args[1]);
		debug("%x", next.in.mode);
	}

	fclose(test);
	return 0;
}
