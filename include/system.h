#pragma once 

#include"cpu.h"
#include"module.h"
#include"types.h"

#define MEM_SIZE 8192
#define IO_PORTS 10

struct system {
	struct mod *ports[IO_PORTS];
	u8 *mem;
};

void system_init(struct system *sys);
void system_load_builtins(struct system *sys);

void system_write(struct system *sys, addr_t addr, u8 data);
u8 system_read(struct system *sys, addr_t addr);

void system_io_write(struct system *sys, u8 which, u8 data);
u8 system_io_read(struct system *sys, u8 which);

void system_set_port(struct system *sys, u8 which, struct mod *module);
void system_reset_port(struct system *sys, u8 which);

void system_destroy(struct system *sys);
