#include<karat/mod.h>
#include<karat/vm/vm.h>
#include<karat/vm/types.h>

#include<stdio.h>

MODULE(
    "karat.con",
    "0.0.1",
    "Kaden Thomas"
);

struct con_data {};

enum {
    WRITE_STR = 0,
    WRITE_CHAR,
};

int on_trap(struct con_data *data, k8_t num, struct vm *vm)
{
    (void) data;
    (void) num;
    (void) vm;
    switch(vm->cpu.regs[0]){
    case WRITE_STR:
        printf((char *)&vm->memory[vm->cpu.regs[1]]);
        break;
    case WRITE_CHAR:
        putchar((char)vm->memory[vm->cpu.regs[1]]);
        break;
    default:
        return 1;
    };
    return 0;
}

int on_module_load(struct con_data **data)
{
    (void) data;
    return 0;
}

int on_module_unload(struct con_data *data)
{
    (void) data;
    return 0;
}
