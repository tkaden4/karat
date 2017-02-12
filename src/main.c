#include<locale.h>
#include<wchar.h>
#include<SDL2/SDL.h>

#include"log.h"
#include"alloc.h"
#include"parse.h"
#include"kprog.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

#define TEST_FILE "test/asm.kt"

#define IGNORE(x)

int main()
{
	setlocale(LC_ALL, "");

	FILE *test = fopen(TEST_FILE, "r");
	err_on(!test, "could not open %s", TEST_FILE);

	struct kprog *prog = kprog_create();
	if(!parse_file(test, prog)){
		puts("succesfully parsed file");
	}else{
		puts("unable to parse file");
	}

	kprog_destroy(prog);
	fclose(test);
	return 0;
}
