#include<locale.h>
#include<wchar.h>
#include<time.h>

#include<unistd.h>

#include<karat/log.h>
#include<karat/alloc.h>
#include<karat/vm/vm.h>
#include<karat/mod.h>
#include<karat/debug.h>
#include<karat/vm/opcode.h>

#define usage() \
    ({ printf("usage: karat [-d] <file>\n"); exit(0); })


#include<karat/test_data.h>

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

    /*
    if(optind != argc - 1){
        usage();
        return 1;
    }
    */

    struct prog_t prog = test_prog(write_port);

    int err = 0;
    if(debug){
	// XXX NULL
        err = idebug(&prog, vm_opts(8096));
    }else{
        struct vm vm;
	// XXX NULL
        vm_run(&vm, vm_opts(8096), &prog);
    } 
    return err;
}
