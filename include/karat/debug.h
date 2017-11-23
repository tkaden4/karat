#pragma once

#include<karat/vm/vm.h>
#include<karat/vm/kprog.h>

/* start interactive debugger */
int idebug(const struct kprog *prog, struct vm_options opts);
