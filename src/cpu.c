#include<string.h>
#include<vm/cpu.h>

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

static inline u8 make_op(u8 type, u8 bitcode)
{
	return (type << 6) | (bitcode & 0x3f);
}

#define INC(cpu) ++(cpu)->pc;

static inline u8 read_byte(struct cpu *cpu, struct kprog *prog)
{
	return prog->program[cpu->pc++];
}

static inline u32 read_long(struct cpu *cpu, struct kprog *prog)
{
	u32 ret = *(long *)&prog->program[cpu->pc];
	cpu->pc += sizeof(u32);
	return ret;
}

static inline u32 value_arg(struct cpu *cpu, struct kprog *prog, u8 spec)
{
	switch(spec){
	case 0:	/* register */
		return cpu->regs[read_long(cpu, prog)];
		break;
	case 1:	/* constant */
		return read_long(cpu, prog);
	case 2:	/* memory */
		puts("memory arguments not supported");
		return 0;
	default:
		puts("this what");
		return (u32)0;
	};
}

static inline u32 *ptr_arg(struct cpu *cpu, struct kprog *prog, u8 spec)
{
	switch(spec){
	case 0:	/* register */
		return &cpu->regs[read_long(cpu, prog)];
		break;
	case 2:	/* memory */
		puts("memory arguments not supported");
		return 0;
	default:
		puts("other what");
		return (u32 *)0;
	};
}

/* destination on the left */
static inline void two_arg(struct cpu *cpu, struct kprog *prog, u8 op)
{
	u8 spec = read_byte(cpu, prog);
	if(op == 0x22){
		*ptr_arg(cpu, prog, spec >> 4) = value_arg(cpu, prog, spec & 0x0f);
	}
}

static inline void one_arg(struct cpu *cpu, struct kprog *prog, u8 op)
{
	u8 spec = read_byte(cpu, prog);
	switch(op){
	case 0x11:	/* goto */
		printf("%u\n", cpu->pc);
		cpu->pc = value_arg(cpu, prog, spec);
		printf("%u\n", cpu->pc);
		break;
	case 0x12:	/* prnt */
		printf("%u\n", value_arg(cpu, prog, spec));
		break;
	case 0x13:	/* inc */
		++*ptr_arg(cpu, prog, spec);
		break;
	};
}

void cpu_step(struct cpu *cpu, struct kprog *prog)
{
	u8 op = read_byte(cpu, prog);
	u8 body = op & 0x3f;
	u8 type = (u8)(op ^ body) >> 6;
	switch(type){
	case 0:
		puts("zero operand");
		break;
	case 1:
		one_arg(cpu, prog, body);
		break;
	case 2:
		two_arg(cpu, prog, body);
		break;
	default:
		printf("unhandled byte 0x%02X\n", op);
	};
}
