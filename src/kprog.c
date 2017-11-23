#include<string.h>

#include<karat/vm/kprog.h>
#include<karat/alloc.h>
#include<karat/log.h>

struct kprog *kprog_create()
{
    struct kprog *ret = s_calloc(1, sizeof(struct kprog));
    ret->entry_point = -1;
    ret->prog_size = 0;
    ret->__size = 0;
    ret->__cap = 0;
    return ret;
}

void kprog_append_bytes(struct kprog *prog, uint32_t data, size_t bytes)
{
    /* fix byte count errors */
    bytes = bytes > sizeof(data) ? sizeof(data) : bytes;
    /* check for buffer overflow */
    if(prog->__size + bytes >= prog->__cap){
        prog->__cap *= 2;
        prog->__cap += bytes;
        prog->program = s_realloc(prog->program, prog->__cap);
    }
    register uint8_t *dptr = (uint8_t *)&data;
    for(size_t i = 1; i <= bytes; ++i){
        prog->program[prog->__size++] = dptr[bytes - i];
    }
}

void kprog_finalize(struct kprog *prog)
{
    prog->prog_size = prog->__size;
    prog->program = s_realloc(prog->program, prog->__size);
    prog->__cap = prog->__size;
}

void kprog_destroy(struct kprog *prog)
{
    err_on(!prog, "program not allocated");
    if(prog->program){
        s_free(prog->program);
    }
    s_free(prog);
}
