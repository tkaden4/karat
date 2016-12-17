#pragma once

#include<stdint.h>
#include<stdlib.h>

#include"karat.h"
#include"types.h"
#include"list.h"

struct mmap_io {
	NODE(struct mmap_io);
	addr_t addr;
	read_cb on_read;
};

#define MEM_SIZE (1 << 16)

struct mem {
	uint8_t *memory;
	struct {
		struct mmap_io *head;
		struct mmap_io *tail;
		size_t size;
	} mmio;
};

API void mem_init(struct mem *mem);
/* write <bytes> amount of bytes of data to location addr */
API void mem_write(struct mem *mem, addr_t addr, uint8_t data);
/* add a serial port to memory, all write instructions
 * will send data through the port */
API void add_mmapped_io(struct mem *mem, addr_t where, read_cb on_read);
/* write to a mmapped io port */
API void mmap_write(struct mem *mem, addr_t addr, uint8_t data);
API void mem_destroy(struct mem *mem);
