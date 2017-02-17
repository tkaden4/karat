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

static inline u16 value_arg(struct cpu *cpu, struct kprog *prog, u8 spec)
{
	switch(spec){
	case 0:	/* register */
		return cpu->regs[read_word(cpu, prog)];
	case 1:	/* constant */
		return read_word(cpu, prog);
	case 2:	/* memory */
		puts("memory arguments not supported");
		return 0;
	default:
		return (u16)0;
	};
}

static inline u16 *ptr_arg(struct cpu *cpu, struct kprog *prog, u8 spec)
{
	switch(spec){
	case 0:	/* register */
	{
		u16 *ret = (u16 *)&cpu->regs[read_word(cpu, prog)];
		return ret;
	}
	case 2:	/* memory */
		puts("memory arguments not supported");
		return 0;
	default:
		return (u16 *)0;
	};
}

static inline void zero_arg(struct cpu *cpu, struct kprog *prog, u8 op)
{
	if(op == 0x01){	/* halt */
		cpu->pc = prog->prog_size;
		return;
	}
}

/* destination on the left */
static inline void two_arg(struct cpu *cpu, struct kprog *prog, u8 op)
{
	u8 spec[2] = {0};
	{
		u8 tmp = read_byte(cpu, prog);
		spec[0] = tmp >> 4;
		spec[1] = tmp & 0x0f;
	}
	switch(op){
	case 0x22:	/* load */
		*ptr_arg(cpu, prog, spec[0]) = value_arg(cpu, prog, spec[1]);
		break;
	case 0x24:	/* cmp */
	{
		const int res = value_arg(cpu, prog, spec[0]) - value_arg(cpu, prog, spec[1]);
		if(res == 0){
			cpu->flags.e = 1;
		}else if(res < 0){
			cpu->flags.n = 1;
		}else{
			cpu->flags.e = 0;
			cpu->flags.n = 0;
		}
		break;
	}
	}
}

static inline void one_arg(struct cpu *cpu, struct kprog *prog, u8 op)
{
	u8 spec = read_byte(cpu, prog);
	switch(op){
	case 0x11:	/* goto */
		cpu->pc = value_arg(cpu, prog, spec);
		break;
	case 0x12:	/* prnt */
		printf("%u\n", value_arg(cpu, prog, spec));
		break;
	case 0x13:	/* inc */
		++*ptr_arg(cpu, prog, spec);
		break;
	case 0x14:	/* dec */
		--*ptr_arg(cpu, prog, spec);
		break;
	case 0x15:	/* beq */
		if(cpu->flags.e){
			cpu->pc = value_arg(cpu, prog, spec);
		}
	case 0x16:	/* bne */
		if(!cpu->flags.e){
			cpu->pc = value_arg(cpu, prog, spec);
		}
		break;
	};
}

/* for dealing with register instructions */
static inline void r_ins(struct cpu *cpu, struct kprog *prog)
{
	(void) cpu; (void) prog;
}

/* for dealing with immediate-mode instructions */
static inline void i_ins(struct cpu *cpu, struct kprog *prog)
{
	(void) cpu; (void) prog;
}

/* for dealing with b-instructions */
static inline void b_ins(struct cpu *cpu, struct kprog *prog)
{
	(void) cpu; (void) prog;
}

void cpu_step(struct cpu *cpu, struct kprog *prog)
{
	u8 op = read_byte(cpu, prog);
	u8 body = op & 0x3f;
	u8 type = (u8)(op ^ body) >> 6;
	switch(type){
	case 0:
		zero_arg(cpu, prog, body);
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
