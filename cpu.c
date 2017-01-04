#include"cpu.h"

#include<string.h>

void cpu_init(struct cpu_state *state)
{
	if(state){
		cpu_reset(state);
	}
}

void cpu_reset(struct cpu_state *state)
{
#if ADDRESS_REGS == GENERAL_REGS
	for(register size_t i = 0; i < GENERAL_REGS; ++i){
		state->regs[i] = 0;
	}
#else
	memset(state->regs, 0, GENERAL_REGS);
#endif
}
