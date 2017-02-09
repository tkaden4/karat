#pragma once

#include<stdio.h>
#include"kprog.h"

const wchar_t *get_perr();
/* parse file into bytecode */
int parse_file(FILE *f, struct kprog *res);
