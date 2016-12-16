#pragma once

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

#include"karat.h"

#define fprefix(file, prefix, fmt, ...) \
	fprintf(file, prefix "::" fmt "\n", ##__VA_ARGS__)

#ifdef DEBUG 
#define fdebug(file, fmt, ...) \
	fprefix(file, "DEBUG", "[  " fmt "  ]", ##__VA_ARGS__)
#else
#define fdebug(file, fmt, ...)
#endif

#define debug(fmt, ...) fdebug(stdout, fmt, ##__VA_ARGS__)


#define flog(file, fmt, ...) \
	fprefix(file, "LOG", "[  " fmt "  ]", ##__VA_ARGS__)
#define log(fmt, ...) flog(stdout, fmt, ##__VA_ARGS__)

#define fwarn(fmt, ...) fprefix(file, "WARN", "[  " fmt "  ]", ##__VA_ARGS__)
#define warn(fmt, ...) fwarn(stdout, fmt, ##__VA_ARGS__)
#define warn_on(condition, fmt, ...) \
	if(condition){ warn(fmt, ##__VA_ARGS__); }

#define ferr(file, fmt, ...)  \
	fprefix(file, "ERR", "[  " fmt "  ]", ##__VA_ARGS__); exit(1);
