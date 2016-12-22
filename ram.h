#pragma once

#include<stdint.h>
#include<stdlib.h>

#include"karat.h"
#include"types.h"
#include"list.h"

#define MEM_SIZE (1 << 16)

struct mem {
	uint8_t *memory;
};

void mem_init(struct mem *mem);
/* write <bytes> amount of bytes of data to location addr */
void mem_write(struct mem *mem, addr_t addr, uint8_t data);
void mem_destroy(struct mem *mem);
