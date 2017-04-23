#include<string.h>
#include<log.h>
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

static inline u8 *read_byte(struct cpu *cpu, struct kprog *prog)
{
	return &prog->program[cpu->pc++];
}

static inline u16 *read_word(struct cpu *cpu, struct kprog *prog)
{
	u16 *ret = (u16 *)&prog->program[cpu->pc];
	cpu->pc += sizeof(u16);
	return ret;
}

static inline u32 *read_long(struct cpu *cpu, struct kprog *prog)
{
	u32 *ret = (u32 *)&prog->program[cpu->pc];
	cpu->pc += sizeof(u32);
	return ret;
}

void cpu_step(struct cpu *cpu, struct kprog *prog)
{
	union opcode op = *(union opcode *)&prog->program[cpu->pc];
	switch(op.I){
	case 0x00:	/* halt */
		cpu->pc = prog->prog_size;
		break;
	case 0x12:	/* bitwise xor */
		cpu->regs[op.r.A] = cpu->regs[op.r.B] ^ cpu->regs[op.r.C];
		break;
	case 0x21:	/* addiu */
		cpu->regs[op.i.A] = cpu->regs[op.i.B] + op.i.Cx;
		break;
	case 0x22:	/* loadk */
		cpu->regs[op.i.A] = op.i.Cx;
		break;
	case 0x29:	/* prntr */
		printf("%d\n", cpu->regs[op.i.A]);
		break;
	case 0x2A:	/* prntv */
		printf("%d\n", op.i.Cx);
		break;
	case 0x2B:	/* beq */
		if(cpu->regs[op.i.A] == cpu->regs[op.i.B]){
			cpu->pc = op.i.Cx;
		}
		break;
	case 0x2C:	/* bne */
		if(cpu->regs[op.i.A] != cpu->regs[op.i.B]){
			cpu->pc = op.i.Cx-sizeof(union opcode);
		}
		break;
	case 0x2D:	/* jmp */
		cpu->pc = op.b.Ax;
		break;
	default:
		err("unimplemented opcode 0x%X", op.I);
		break;
	};
	cpu->pc += 4;
}
