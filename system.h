#pragma once 
#include"cpu.h"
#include"ram.h"
#include"module.h"
#include"types.h"

#define IO_PORTS 10

struct system {
	struct cpu_state cpu;
	struct mem memory;
	struct mod *ports[IO_PORTS];
};

void system_init(struct system *sys);
void set_port(struct system *sys, io_t which, struct mod *module);
void reset_port(struct system *sys, io_t which);
void system_destroy(struct system *sys);
