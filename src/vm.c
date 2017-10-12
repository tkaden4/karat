#include<vm/vm.h>

#include<log.h>
#include<alloc.h>
#include<vm/opcode.h>
#include<vm/cpu.h>

#define rmode_bin(cpu, opcode, op) \
    (cpu)->regs[opcode.r.A] = (cpu)->regs[opcode.r.B] op (cpu)->regs[opcode.r.C]; break

#define imode_bin(cpu, opcode, op) \
    (cpu)->regs[opcode.i.A] = (cpu)->regs[opcode.i.B] op opcode.i.Cx; break

#define bmode_cmp(cpu, opcode, cmp) \
    ({ \
        (cpu)->pc = (cpu)->regs[opcode.i.A] cmp (cpu)->regs[opcode.i.B] \
        ? opcode.i.Cx : (cpu)->pc; break; \
    })

#define push(vm, value) \
    ({  \
        *(typeof(value) *)((vm)->memory + (vm)->cpu.sp) = (value); \
        (vm)->cpu.sp += sizeof(value); \
     })

#define pop(vm, type) \
    ({ \
        (vm)->cpu.sp -= sizeof(type); \
        *(type *)((vm)->memory + (vm)->cpu.sp); \
     })

static inline void vm_step(struct vm *vm)
{
    struct cpu *cpu = &vm->cpu;
    const struct kprog *prog = vm->prog;
    union opcode op = *(union opcode *)&prog->program[cpu->pc];
    cpu->pc += sizeof(union opcode);
    switch(op.I){
    /* No-mode instructions */
    case HALT_CODE:     cpu->pc = prog->prog_size; break;
    case RET_CODE:      cpu->pc = pop(vm, reg_t); break;
    case DUP_CODE: 
    {
        reg_t val = pop(vm, reg_t);
        push(vm, val);
        push(vm, val);
    }
        break;
    /* Register-mode instructions */
    case JMPR_CODE:     cpu->pc = cpu->regs[op.i.A]; break;
    case READ_CODE:     cpu->regs[op.r.A] = fgetc(stdin); break;
    case MODR_CODE:     rmode_bin(cpu, op, %);
    case XORR_CODE:     rmode_bin(cpu, op, ^);
    case SUBS_CODE:     rmode_bin(cpu, op, -);
    case ADDS_CODE:     rmode_bin(cpu, op, +);
    case MULS_CODE:     rmode_bin(cpu, op, *);
    case PUSHR_CODE:    push(vm, cpu->regs[op.i.A]); break;
    case POPR_CODE:     cpu->regs[op.i.A] = pop(vm, reg_t); break;
    /* Intermediate-mode instructions */
    case ADDIU_CODE:    imode_bin(cpu, op, +);
    case SUBIS_CODE:    imode_bin(cpu, op, -);
    case LOADK_CODE:    cpu->regs[op.i.A] = op.i.Cx; break;
    case LOADR_CODE:    cpu->regs[op.r.A] = cpu->regs[op.r.B]; break;
    case PUTR_CODE:     printf("%d\n", cpu->regs[op.i.A]); break;
    case PUTV_CODE:     printf("%d\n", op.i.Cx); break;
    /* Branch-mode instructions */
    case BEQ_CODE:      bmode_cmp(cpu, op, ==);
    case BNE_CODE:      bmode_cmp(cpu, op, !=);
    case BGT_CODE:      bmode_cmp(cpu, op, >);
    case BLT_CODE:      bmode_cmp(cpu, op, <);
    case JMP_CODE:      cpu->pc = op.b.Ax; break;
    case CALL_CODE:
        push(vm, cpu->pc);
        cpu->pc = op.b.Ax;
        break;
    default:
        if(op.I >= MAX_OPCODES){
            err("mangled opcode out of range: 0x%02X", op.I);
        }else{
            err("unimplemented opcode 0x%02X (%ls)", op.I, op_defs[op.I].mnemonic);
        }
    };
}

void vm_run(struct vm *vm, const struct kprog *prog)
{
    memset(vm, 0, sizeof(*vm));
    vm->prog = prog;
    vm->memory = s_malloc(1024); 
    memset(vm->memory, 0, 1024);

    vm->cpu.pc = prog->entry_point;
    while(vm->cpu.pc < prog->prog_size){
        vm_step(vm);
    }

    s_free(vm->memory);
}
