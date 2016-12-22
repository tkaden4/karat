#include"ram.h"

#include<string.h>

#include"mem.h"
#include"log.h"

void mem_init(struct mem *mem)
{
	/* allocate the memory, calloc will zero it by defualt */
	err_on(!mem, "memory struct not initialized");
	mem->memory = s_calloc(MEM_SIZE, 1);
}


void mem_write(struct mem *mem, addr_t addr, uint8_t data)
{
	err_on(!mem, "passing unallocated struct mem");
	mem->memory[addr] = data;
}

void mem_destroy(struct mem *mem)
{
	err_on(!mem, "passing unallocated struct mem");
	warn_on(!mem->memory, "memory inside of mem struct not allocated");
	if(mem->memory){
		s_free(mem->memory);
	}
}
