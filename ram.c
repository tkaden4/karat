#include"ram.h"

#include<string.h>

#include"mem.h"
#include"log.h"

API void mem_init(struct mem *mem)
{
	/* allocate the memory, calloc will zero it by defualt */
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

	/* check if already exists, non-fatal, but we still warn the user */
	struct mmap_io *found = NULL;
	LIST_FIND(struct mmap_io, mem->mmio.head, found, (walk->addr == where));
	warn_on(found, "a module already has ownership of 0x%X", where);
	/* add a new mmap_io struct to the list */
	struct mmap_io *add = s_malloc(sizeof(struct mmap_io));
	add->addr = where;
	add->on_read = on_read;
	/* one time thing, rarely happens */
	if(!mem->mmio.head){
		mem->mmio.head = add;
		mem->mmio.head->next = NULL;
		mem->mmio.tail = mem->mmio.head;
	/* the more common case */
	}else{
		mem->mmio.tail->next = add;
		mem->mmio.tail = mem->mmio.tail->next;
		mem->mmio.tail->next = NULL;
	}
	++mem->mmio.size;
}

API void mmap_write(struct mem *mem, addr_t addr, uint8_t data)
{
	err_on(!mem, "passing unallocated struct mem");
	struct mmap_io *found = NULL;
	LIST_FIND(struct mmap_io, mem->mmio.head, found, (walk->addr == addr));
	if(found){
		CALL_CB(found->on_read, data);
	}else{
		warn("no port found at address 0x%X", addr);
	}
}

API void mem_destroy(struct mem *mem)
{
	err_on(!mem, "passing unallocated struct mem");
	warn_on(!mem->memory, "memory inside of mem struct not allocated");
	if(mem->memory){
		s_free(mem->memory);
	}
	if(mem->mmio.head){
		LIST_FREELOOP(struct mmap_io, mem->mmio.head, each, {
			s_free(each);
		});
	}
}
