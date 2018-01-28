#include<locale.h>
#include<wchar.h>
#include<time.h>

#include<unistd.h>

#include<karat/log.h>
#include<karat/alloc.h>
#include<karat/vm/vm.h>
#include<karat/mod.h>
#include<karat/debug.h>

#define usage() \
    ({ printf("usage: karat [-d] <file>\n"); exit(0); })

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    int c = EOF;
    bool debug = false;
    while((c = getopt(argc, argv, "o:ld")) != -1){
        switch(c){
        case 'd':
            debug = true;
            break;
        case '?':
            usage();
            return 1;
        }
    }

    if(optind >= argc){
        usage();
        return 1;
    }else if(optind != argc - 1){
        usage();
        return 1;
    }

    const char *prog = argv[optind];

    FILE *program = fopen(prog, "r");
    err_on(!program, "could not open %s", prog);

    int err = 0;

    if(debug){
        err = idebug(NULL, vm_opts(8096));
    }else{
        struct vm vm;
        vm_run(&vm, vm_opts(8096), NULL);
    } 

    fclose(program);
    return err;
}
