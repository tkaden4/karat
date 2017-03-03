#include<locale.h>
#include<wchar.h>
#include<SDL2/SDL.h>

#include<log.h>
#include<alloc.h>
#include<vm/kprog.h>
#include<vm/cpu.h>
#include<parse/parse.h>

#define usage() \
	printf("Usage: karat < file.kt >"); exit(0);

int main(int argc, const char *argv[])
{
	setlocale(LC_ALL, "");

	const char *prog = NULL;
	switch(argc){
	case 2:
		prog = argv[1];
		break;
	default:
		usage();
	};

	FILE *test = fopen(prog, "r");
	err_on(!test, "could not open %s", prog);
	struct kprog *rprog = kprog_create();
	if(!parse_file(test, rprog)){
		struct cpu cpu;
		cpu_init(&cpu);
		cpu.pc = rprog->entry_point;
		while(cpu.pc < rprog->prog_size){
			cpu_step(&cpu, rprog);
		}
	}
	kprog_destroy(rprog);
	fclose(test);
	return 0;
}
