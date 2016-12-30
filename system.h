#pragma once 
#include"cpu.h"
#include"module.h"
#include"types.h"

#define MEM_SIZE 8192
#define IO_PORTS 10
#define INTERNAL_IO_MAX 3

struct system {
	struct cpu_state cpu;
	u8 *mem;
	struct mod *ports[IO_PORTS];
};

void system_init(struct system *sys);
void system_load_builtins(struct system *sys);
void system_io_write(struct system *sys, io_t which, u8 data);
void system_set_port(struct system *sys, io_t which, struct mod *module);
void system_reset_port(struct system *sys, io_t which);
void system_destroy(struct system *sys);
