#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include<readline/readline.h>

#include<karat/debug.h>
#include<karat/list.h>
#include<karat/alloc.h>
#include<karat/vm/vm.h>
#include<karat/vm/opcode.h>

#define s_eq(x, y) !strcmp(x, y)

struct debug_ctx {
    struct vm vm;
};

static char *trim(char *str, char c)
{
    while(*str && *str == c) ++str;
    size_t len = strlen(str);
    while(len-- && str[len] == c);
    str[len + 1] = '\0';
    return str;
}

#define opcode_at(prog, i) ((union opcode *)(&(prog)->program[i]))

static inline const struct op_def *find_op_by_code(const union opcode *op)
{
    /* only way that this will work is with bitwise and */
    return &op_defs[op->op & 0b111111];
}

/* disassemble opcode */
static inline void dis_opcode(const union opcode *op)
{
#define put_reg(reg) printf("r%d ", reg);
    const struct op_def *def = find_op_by_code(op);
    printf("%ls", def->mnemonic);
    amode_t mode = def->argmode;
    switch(MODETYPE(mode)){
    case iABCDF:
        putchar(' ');
        if(HAS_ARG(mode, A_ARG)) put_reg(op->r.A);
        if(HAS_ARG(mode, B_ARG)) put_reg(op->r.B);
        if(HAS_ARG(mode, C_ARG)) put_reg(op->r.C);
        if(HAS_ARG(mode, D_ARG)) put_reg(op->r.D);
        if(HAS_ARG(mode, F_ARG)) put_reg(op->r.F);
        break;
    case iABCx:
        putchar(' ');
        if(HAS_ARG(mode, A_ARG)) put_reg(op->i.A);
        if(HAS_ARG(mode, B_ARG)) put_reg(op->i.B);
        if(HAS_ARG(mode, Cx_ARG)) printf("$%04lX", (size_t)op->i.Cx);
        break;
    case iAx:
        putchar(' ');
        if(HAS_ARG(mode, Ax_ARG)) printf("$%08lX", (size_t)op->b.Ax);
        break;
    default:
        break;
    };
#undef put_reg
}

/* disassemble */
static inline void dis(struct debug_ctx *d)
{
    for(size_t i = 0; i < d->vm.prog->prog_size; i += sizeof(union opcode)){
        printf("$%08lX ", i);
        dis_opcode(opcode_at(d->vm.prog, i));
        puts("");
    }
}

/* show help */
static inline void help()
{
#define cmd(cmd, desc) printf("    %-8s%s\n", cmd, desc);
    puts("commands");
    cmd("help", "display help");
    cmd("exit", "exit kdb");
    cmd("dis", "disassemble program");
#undef cmd
}

/* deal with commands that are more complicated (more than 1 word) */
static inline void longcmd(const char *str, struct debug_ctx *d)
{
    (void) str;
    (void) d;
    /* command is unknown at this point */
    printf("kdb: unrecognized command \'%s\'\n", str);
}

int idebug(const struct kprog *prog, struct vm_options opts)
{
    struct debug_ctx d;
    cpu_init(&d.vm.cpu, prog->entry_point, 0);
    d.vm.prog = prog;
    d.vm.memory = s_calloc(1, opts.memory_size);

    char *line = NULL;
    bool running = true;
    while(running && (line = readline("kdb> "))){
        if(!s_eq(line, "")){
            const char *trimmed = trim(line, ' ');
            if(s_eq(trimmed, "exit")){
                running = false;
            }else if(s_eq(trimmed, "dis")){
                dis(&d);
            }else if(s_eq(trimmed, "help")){
                help();
            }else{
                longcmd(trimmed, &d);
            }
        }
        s_free(line);
    }

    s_free(d.vm.memory);
    return 0;
}
