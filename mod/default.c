#include<karat/ktypes.h>
#include<karat/vm/opcode.h>
#include<karat/mod.h>

#include<stdio.h>

int opcode_init(struct kmod *mod)
{
    (void) mod;
    puts("printing from module");
    puts("foobar");
    return 0;
}

int opcode_unload(struct kmod *mod)
{
    (void) mod;
    return 0;
}
