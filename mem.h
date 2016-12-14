#pragma once

/*      mem.h 
 * defines methods for checked memory allocation 
 */

#include<stdio.h>
#include<stdlib.h>

#define TRACK_ALLOC 0

void *s_malloc(size_t bytes);    
void *s_calloc(size_t size, size_t bytes);
void *s_realloc(void *data, size_t size);
void s_free(void *data);    
