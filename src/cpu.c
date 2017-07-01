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

void cpu_run(struct cpu *cpu, struct kprog *prog)
{
    cpu->pc = prog->entry_point;
    while(cpu->pc < prog->prog_size){
        cpu_step(cpu, prog);
    }
}

void cpu_step(struct cpu *cpu, struct kprog *prog)
{
	union opcode op = *(union opcode *)&prog->program[cpu->pc];
	cpu->pc += 4;
	switch(op.I){
	case 0x00:	/* halt */
		cpu->pc = prog->prog_size;
		break;
    case 0x1B:  /* jmpr */
        cpu->pc = cpu->regs[op.i.A];
        break;
    case 0x1C:  /* remr */
        cpu->regs[op.r.A] = cpu->regs[op.r.B] % cpu->regs[op.r.C];
        break;
	case 0x12:	/* bitwise xor */
		cpu->regs[op.r.A] = cpu->regs[op.r.B] ^ cpu->regs[op.r.C];
		break;
    case 0x13:  /* subs */
        cpu->regs[op.r.A] = cpu->regs[op.r.B] - cpu->regs[op.r.C];
        break;
    case 0x10:  /* adds */
        cpu->regs[op.r.A] = cpu->regs[op.r.B] + cpu->regs[op.r.C];
        break;
	case 0x21:	/* addiu */
		cpu->regs[op.i.A] = cpu->regs[op.i.B] + op.i.Cx;
		break;
	case 0x22:	/* loadk */
		cpu->regs[op.i.A] = op.i.Cx;
		break;
    case 0x19:  /* loadr */
        cpu->regs[op.r.A] = cpu->regs[op.r.B];
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
			cpu->pc = op.i.Cx;
		}
		break;
	case 0x30:	/* jmp */
		cpu->pc = op.b.Ax;
		break;
    case 0x2E:  /* bgt */
        cpu->pc = cpu->regs[op.i.A] > cpu->regs[op.i.B] ? op.i.Cx : cpu->pc;
        break;
    case 0x2F:  /* blt */
        cpu->pc = cpu->regs[op.i.A] < cpu->regs[op.i.B] ? op.i.Cx : cpu->pc;
        break;
	default:
        err("unimplemented opcode 0x%2X", op.I);
	};
}
