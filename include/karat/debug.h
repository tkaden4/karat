#pragma once

#include<karat/vm/vm.h>
#include<karat/vm/types.h>

/* start interactive debugger */
int idebug(const prog_t prog, struct vm_options opts);
