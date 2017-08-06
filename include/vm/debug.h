#pragma once

#include<vm/cpu.h>
#include<wchar.h>

struct debug_state {
    /* cpu we are hooked into */
    struct cpu *running_cpu;
}

void debug_start(struct debug_state *state, struct cpu *cpu);
void debug_step(struct debug_state *state);
void debug_end(struct debug_state *state);
