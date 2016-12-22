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
	for(size_t i = 0; i < ADDRESS_REGS; ++i){
		state->gregs[i] = 0;
		state->aregs[i] = 0;
	}
#else
	memset(state->gregs, 0, GENERAL_REGS);
	memset(state->aregs, 0, ADDRESS_REGS);
#endif
	state->pc = 0;
}
