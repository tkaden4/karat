#include<string.h>
#include<vm/cpu.h>
#include<vm/opcode.h>

void cpu_init(struct cpu *state)
{
	if(state){
		cpu_reset(state);
	}
}

void cpu_reset(struct cpu *state)
{
	memset(state, 0, sizeof(struct cpu));
}

#define INC(cpu) ++(cpu)->pc;

static inline u8 read_byte(struct cpu *cpu, struct kprog *prog)
{
	return prog->program[cpu->pc++];
}

static inline u16 read_word(struct cpu *cpu, struct kprog *prog)
{
	u16 ret = *(u16 *)&prog->program[cpu->pc];
	cpu->pc += sizeof(u16);
	return ret;
}

static inline u32 read_long(struct cpu *cpu, struct kprog *prog)
{
	u32 ret = *(u32 *)&prog->program[cpu->pc];
	cpu->pc += sizeof(u32);
	return ret;
}

static inline void r_ins(struct cpu *cpu, struct kprog *prog, u32 op)
{
	(void) cpu; (void) prog; (void) op;
}

static inline void i_ins(struct cpu *cpu, struct kprog *prog, u32 op)
{
	(void) cpu; (void) prog; (void) op;
}

static inline void b_ins(struct cpu *cpu, struct kprog *prog, u32 op)
{
	(void) cpu; (void) prog; (void) op;
}

void cpu_step(struct cpu *cpu, struct kprog *prog)
{
	u32 op = read_long(cpu, prog);
	(void) op;
}
