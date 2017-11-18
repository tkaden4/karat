#pragma once

/* stdlib files */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define zmem(x) memset((void *)(&(x)), 0, sizeof((x)))

#define s_alloc(type) s_calloc(1, sizeof(type))

#ifndef KARAT_PROD
void *s_malloc(size_t bytes);    
void *s_calloc(size_t nitems, size_t member);
void *s_realloc(void *data, size_t size);
void s_free(void *data);    
#else
#define s_malloc malloc
#define s_calloc calloc
#define s_realloc realloc
#define s_free free
#endif
