#pragma once

#include<stdio.h>
#include"kprog.h"

/* parse file into bytecode */
int parse_file(FILE *f, struct kprog *res);
