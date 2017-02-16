#pragma once

#include<string.h>

#define RBUFF_ELEM_INDEX(start, size, n) (((start) + (n)) % (size))

#define RBUFF_DECL(name, type, size) \
struct name { type buff[size]; size_t start; size_t len; }

#define RBUFF_IMPL(name, type, size) \
static void name ## _init(struct name *__buff) \
{ \
	memset(__buff->buff, 0, sizeof(type)*size); \
	__buff->start = 0; \
	__buff->len = 0; \
} \
\
static type *name ## _back(struct name *__buff) \
{ \
	return &__buff->buff[RBUFF_ELEM_INDEX(__buff->start, size, __buff->len - 1)]; \
}; \
\
static type *name ## _front(struct name *__buff) \
{ \
	return &__buff->buff[RBUFF_ELEM_INDEX(__buff->start, size, 0)]; \
} \
static type *name ## _push_back(struct name *__buff) \
{ \
	if(__buff->len == (size)){ \
		return NULL; \
	} \
	++__buff->len; \
	type *ret = name ## _back(__buff); \
	memset(ret, 0, sizeof(type));\
	return ret; \
} \
static type *name ## _push_front(struct name *__buff) \
{ \
	if(__buff->len == (size)){ \
		return NULL; \
	} \
	__buff->start = RBUFF_ELEM_INDEX(__buff->start, size, __buff->len);\
	++__buff->len; \
	return name ## _front(__buff); \
} \
static type *name ## _pop_back(struct name *__buff) \
{ \
	if(__buff->len == 0){ \
		return NULL; \
	} \
	type *ret = name ## _back(__buff); \
	--__buff->len; \
	return ret; \
} \
static type *name ## _pop_front(struct name *__buff) \
{ \
	if(__buff->len == 0){ \
		return NULL; \
	} \
	type *ret = name ## _front(__buff); \
	__buff->start = RBUFF_ELEM_INDEX(__buff->start, size, 1); \
	--__buff->len; \
	return ret; \
} \
static type *name ## _elem_n(struct name *__buff, size_t n) \
{ \
	if(__buff->len == 0){ \
		return NULL; \
	} \
	return &__buff->buff[RBUFF_ELEM_INDEX(__buff->start, size, n)];\
} \
static type *name ## _elem(struct name *__buff) \
{ \
	if(__buff->len == 0){ \
		return NULL; \
	} \
	return &__buff->buff[RBUFF_ELEM_INDEX(__buff->start, size, 0)];\
}

