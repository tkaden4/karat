#include"mem.h"

#define CHECK_RET(x) \
    { \
        if(!x){\
            fprintf(stderr, "FATAL::> Unable to allocate memory\n");\
            exit(1);\
        }\
        return x;\
    }

void *s_malloc(size_t bytes)
{
#if TRACK_ALLOC
	static int i = 0;
	printf("%d bytes allocated\n", (i += bytes));
#endif
    void *ret = malloc(bytes);
    CHECK_RET(ret);
}

void *s_calloc(size_t size, size_t bytes)
{
    void *ret = calloc(size, bytes);
    CHECK_RET(ret);
}

void *s_realloc(void *data, size_t size)
{
    void *ret = realloc(data, size);
    CHECK_RET(ret);
}

void s_free(void *bytes)
{
#if TRACK_ALLOC
	static int i = 0;
	printf("%d frees\n", ++i);
#endif
    if(!bytes){
        fprintf(stderr, "FATAL::> Attempt to free unallocated memory\n");
		exit(1);
        return;
    }
    free(bytes);
}
