#pragma once

#include"system.h"
#include"cpu.h"
#include"list.h"
#include"opcode.h"
#include"kprog.h"


/* visit all results with one function */
const wchar_t *get_perr();
int parse_file(FILE *f, struct kprog *res);
