#include<vm/vm.h>

#include<log.h>
#include<alloc.h>
#include<vm/opcode.h>
#include<vm/cpu.h>

void cpu_init(struct cpu *state)
{
    if(state){
        memset(state, 0, sizeof(struct cpu));
    }
}

#define rmode_bin3(cpu, opcode, op) \
    (cpu)->regs[opcode.r.A] = (cpu)->regs[opcode.r.B] op (cpu)->regs[opcode.r.C]; break

#define imode_bin3(cpu, opcode, op) \
    (cpu)->regs[opcode.i.A] = (cpu)->regs[opcode.i.B] op opcode.i.Cx; break

#define bmode_bin3(cpu, opcode, cmp) \
    (cpu)->pc = (cpu)->regs[opcode.i.A] cmp (cpu)->regs[opcode.i.B] \
    ? opcode.i.Cx : (cpu)->pc; break

void vm_step(struct vm *vm)
{
    struct cpu *cpu = &vm->cpu;
    struct kprog *prog = vm->prog;
    union opcode op = *(union opcode *)&prog->program[cpu->pc];
    cpu->pc += sizeof(union opcode);
    switch(op.I){
    /* No-mode instructions */
    case HALT_CODE:     cpu->pc = prog->prog_size; break;
    /* Register-mode instructions */
    case JMPR_CODE:     cpu->pc = cpu->regs[op.i.A]; break;
    case MODR_CODE:     cpu->regs[op.r.A] = cpu->regs[op.r.B] % cpu->regs[op.r.C]; break;
    case READ_CODE:     cpu->regs[op.r.A] = fgetc(stdin); break;
    case XORR_CODE:     rmode_bin3(cpu, op, ^);
    case SUBS_CODE:     rmode_bin3(cpu, op, -);
    case ADDS_CODE:     rmode_bin3(cpu, op, +);
    case MULS_CODE:     rmode_bin3(cpu, op, *);
    /* Intermediate-mode instructions */
    case ADDIU_CODE:    imode_bin3(cpu, op, +);
    case SUBIS_CODE:    imode_bin3(cpu, op, -);
    case LOADK_CODE:    cpu->regs[op.i.A] = op.i.Cx; break;
    case LOADR_CODE:    cpu->regs[op.r.A] = cpu->regs[op.r.B]; break;
    case PUTR_CODE:     printf("%d\n", cpu->regs[op.i.A]); break;
    case PUTV_CODE:     printf("%d\n", op.i.Cx); break;
    /* Branch-mode instructions */
    case BEQ_CODE:      bmode_bin3(cpu, op, ==);
    case BNE_CODE:      bmode_bin3(cpu, op, !=);
    case BGT_CODE:      bmode_bin3(cpu, op, >);
    case BLT_CODE:      bmode_bin3(cpu, op, <);
    case JMP_CODE:      cpu->pc = op.b.Ax; break;
    default:
        err("unimplemented opcode 0x%02X", op.I);
    };
}

void vm_run(struct vm *vm, struct kprog *prog)
{
    cpu_init(&vm->cpu);
    vm->prog = prog;
    vm->memory = s_malloc(1024); 

    vm->cpu.pc = prog->entry_point;
    while(vm->cpu.pc < prog->prog_size){
        vm_step(vm);
    }

    s_free(vm->memory);
}
