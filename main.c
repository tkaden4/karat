#include<locale.h>
#include<wchar.h>
#include<SDL2/SDL.h>

#include"log.h"
#include"system.h"
#include"alloc.h"
#include"list.h"
#include"parse.h"
#include"opcode.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

#define TEST_FILE "test/asm.kt"

#define IGNORE(x)

#ifdef KDEBUG
void CONSTRUCTOR __opcodes_init()
{
	printf("OPCODES : %d total %lu bytes\n", MAXIMUM_OP, sizeof(ops));
	for(size_t i = 0; i < MAXIMUM_OP; ++i){
		printf("\t0x%x -> %ls\n", ops[i].code, ops[i].mnemonic);
	}
}
#endif

int main()
{
	setlocale(LC_ALL, "");

	FILE *test = fopen(TEST_FILE, "r");
	err_on(!test, "could not open %s", TEST_FILE);

	/* parse file into bytecode */
	struct presult line_data;
	wchar_t line[100];
	while(fgetws(line, 100, test) && parse_line(line, &line_data)){

	}

	fclose(test);
	return 0;
}
