#pragma once

/*      mem.h 
 * defines methods for checked memory allocation 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define zstruct(x) memset((void *)(&(x)), 0, sizeof((x)))

#define s_alloc(type) s_malloc(sizeof(type))

void *s_malloc(size_t bytes);    
void *s_calloc(size_t size, size_t bytes);
void *s_realloc(void *data, size_t size);
void s_free(void *data);    
