#pragma once

#include<karat/vm/types.h>

typedef void(*trap_handler)(struct vm *);

struct trap {
    k16_t code;
    trap_handler handler;
};
