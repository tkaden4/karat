#include"ram.h"

#include<string.h>

#include"mem.h"
#include"log.h"

API void mem_init(struct mem *mem)
{
	mem->memory = s_calloc(MEM_SIZE, 1);
	mem->mmio.head = NULL;
	mem->mmio.tail = NULL;
	mem->mmio.size = 0;
}


API void mem_write(struct mem *mem, addr_t addr, uint8_t data)
{
	err_on(!mem, "passing unallocated struct mem");
	mem->memory[addr] = data;
}

API void add_mmapped_io(struct mem *mem, addr_t where, read_cb on_read)
{
	err_on(!mem, "passing unallocated struct mem");
	struct mmap_io *add = s_malloc(sizeof(struct mmap_io));
	add->addr = where;
	add->on_read = on_read;
	if(!mem->mmio.head){
		mem->mmio.head = add;
		mem->mmio.head->next = NULL;
		mem->mmio.tail = mem->mmio.head;
	}else{
		mem->mmio.tail->next = add;
		mem->mmio.tail = mem->mmio.tail->next;
		mem->mmio.tail->next = NULL;
	}
}

API void mmap_write(struct mem *mem, addr_t addr, uint8_t data)
{
	if(!mem){
		return;
	}
	struct mmap_io *found = NULL;
	LIST_FIND(struct mmap_io, mem->mmio.head, found, (walk->addr == addr));
	if(found){
		CALL_CB(found->on_read, data);
	}else{
		
	}
}

API void mem_destroy(struct mem *mem)
{
	if(!mem){
		return;
	}
	if(mem->memory){
		s_free(mem->memory);
	}
	if(mem->mmio.head){
		LIST_FREELOOP(struct mmap_io, mem->mmio.head, each, {
			s_free(each);
		});
	}
}
