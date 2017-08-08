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
    cpu->pc += sizeof(union opcode);
    switch(op.I){
    case HALT_CODE:
        cpu->pc = prog->prog_size; break;
    case JMPR_CODE:
        cpu->pc = cpu->regs[op.i.A]; break;
    case MODR_CODE:
        cpu->regs[op.r.A] = cpu->regs[op.r.B] % cpu->regs[op.r.C]; break;
    case READ_CODE:
        cpu->regs[op.r.A] = fgetc(stdin); break;
    case XOR_CODE:
        cpu->regs[op.r.A] = cpu->regs[op.r.B] ^ cpu->regs[op.r.C]; break;
    case SUBS_CODE:
        cpu->regs[op.r.A] = cpu->regs[op.r.B] - cpu->regs[op.r.C]; break;
    case ADDS_CODE:
        cpu->regs[op.r.A] = cpu->regs[op.r.B] + cpu->regs[op.r.C]; break;
    case ADDIU_CODE:
        cpu->regs[op.i.A] = cpu->regs[op.i.B] + op.i.Cx; break;
    case SUBIS_CODE:
        cpu->regs[op.i.A] = cpu->regs[op.i.B] - op.i.Cx; break;
    case LOADK_CODE:
        cpu->regs[op.i.A] = op.i.Cx; break;
    case LOADR_CODE:
        cpu->regs[op.r.A] = cpu->regs[op.r.B]; break;
    case PUTR_CODE:
        printf("%d\n", cpu->regs[op.i.A]); break;
    case PUTV_CODE:
        printf("%d\n", op.i.Cx); break;
    case BEQ_CODE:
        cpu->pc = cpu->regs[op.i.A] == cpu->regs[op.i.B] ? op.i.Cx : cpu->pc; break;
    case BNE_CODE:
        cpu->pc = cpu->regs[op.i.A] != cpu->regs[op.i.B] ? op.i.Cx : cpu->pc; break;
    case JMP_CODE:
        cpu->pc = op.b.Ax; break;
    case BGT_CODE:
        cpu->pc = cpu->regs[op.i.A] > cpu->regs[op.i.B] ? op.i.Cx : cpu->pc; break;
    case BLT_CODE:
        cpu->pc = cpu->regs[op.i.A] < cpu->regs[op.i.B] ? op.i.Cx : cpu->pc; break;
    default:
        err("unimplemented opcode 0x%2X", op.I);
    };
}
