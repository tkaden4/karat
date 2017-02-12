#include"kprog.h"

#include<string.h>
#include"alloc.h"
#include"log.h"

struct kprog *kprog_create()
{
	struct kprog *ret = s_calloc(1, sizeof(struct kprog));
	return ret;
}

void kprog_destroy(struct kprog *prog)
{
	err_on(!prog, "program not allocated");
	//s_free(prog->program);
	s_free(prog);
}
