#include<locale.h>
#include<wchar.h>
#include<time.h>

#include<log.h>
#include<alloc.h>
#include<vm/kprog.h>
#include<vm/cpu.h>
#include<parse/parse.h>

#define usage() \
    printf("Usage: karat < file.k >"); exit(0);

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

    printf("assembling \"%s\"\n", prog);
    const time_t start = clock();
    struct kprog *rprog = kprog_create();
    if(!parse_file(test, rprog)){
        printf("assembly took %lfms\n", ((double)clock()-start)/CLOCKS_PER_SEC);
        printf("running program (%lu bytes)...\n", rprog->prog_size);
        /* Run the program */
        struct cpu cpu = {};
        cpu_init(&cpu);
        cpu_run(&cpu, rprog);
    }

    kprog_destroy(rprog);
    fclose(test);
    return 0;
}
