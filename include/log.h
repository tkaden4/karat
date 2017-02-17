#pragma once

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

#include"traits.h"

#define fprefix(file, prefix, fmt, ...) \
	fprintf(file,  "[ " prefix " ] " fmt " [ %s : %s : %d ]\n", \
	##__VA_ARGS__, __FILE__, __func__, __LINE__ )

#ifdef KDEBUG 
#define fdebug(file, fmt, ...) \
	fprefix(file, "DEBUG", fmt, ##__VA_ARGS__)
#else
#define fdebug(file, fmt, ...)
#endif

#define debug(fmt, ...) fdebug(stdout, fmt, ##__VA_ARGS__)

#define fwarn(file, fmt, ...) \
	fprefix(file, "WARNING", fmt, ##__VA_ARGS__)
#define warn(fmt, ...) fwarn(stdout, fmt, ##__VA_ARGS__)
#define warn_on(condition, fmt, ...) \
	if(condition){ warn(fmt, ##__VA_ARGS__); }
#define warn_ret(condition, fmt, ...) \
	warn_on(condition, fmt, ##__VA_ARGS__); if(condition) return

#define warn_alloc(mem) warn_ret(!mem, "\"%s\" not allocated", # mem)

#define ferr(file, fmt, ...)  \
	fprefix(file, "ERROR", fmt, ##__VA_ARGS__); exit(1);
#define err(fmt, ...) ferr(stdout, fmt, ##__VA_ARGS__)
#define err_on(condition, fmt, ...) \
	if(condition){ err(fmt, ##__VA_ARGS__); }
