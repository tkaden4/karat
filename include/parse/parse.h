#pragma once

/* stdlib files */
#include<stdio.h>
/* karat files */
#include<vm/kprog.h>

/* parse file into bytecode */
int parse_file(FILE *f, struct kprog *res);
