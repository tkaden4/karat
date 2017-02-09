#include<locale.h>
#include<wchar.h>
#include<SDL2/SDL.h>

#include"log.h"
#include"system.h"
#include"alloc.h"
#include"list.h"
#include"parse.h"
#include"opcode.h"
#include"kprog.h"
#include"smap.h"

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

#define TEST_FILE "test/asm.kt"

#define IGNORE(x)

#ifdef KDEBUG
void CONSTRUCTOR __opcodes_init()
{
	printf("OPCODES : %d total %lu bytes\n", MAXIMUM_OP, sizeof(op_defs));
	for(size_t i = 0; i < MAXIMUM_OP; ++i){
		printf("\t0x%x -> %ls\n", op_defs[i].code, op_defs[i].mnemonic);
	}
}
#endif

int main()
{
	setlocale(LC_ALL, "");

	struct smap *testm = smap_create(wcscmp);
	printf("%ls\n", (wchar_t *)smap_lookup(testm, L"key"));

	smap_insert(testm, L"key", wcsdup(L"value"));
	smap_insert(testm, L"key", wcsdup(L"fuck"));

	printf("%ls\n", (wchar_t *)smap_lookup(testm, L"key"));
	smap_destroy(testm);
	return 0;

	FILE *test = fopen(TEST_FILE, "r");
	err_on(!test, "could not open %s", TEST_FILE);

	struct kprog *prog = kprog_create();
	if(parse_file(test, prog)){
		kprog_destroy(prog);
		return 1;
	}
	kprog_destroy(prog);

	fclose(test);
	return 0;
}
