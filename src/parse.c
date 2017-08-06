/* stdlib files */
#include<stdio.h>
#include<wctype.h>
#include<ctype.h>
#include<wchar.h>
/* karat files */
#include<parse/rbuff.h>
#include<parse/parse.h>
#include<parse/lex.h>
#include<vm/opcode.h>
#include<vm/cpu.h>
#include<list.h>
#include<ktypes.h>
#include<alloc.h>
#include<log.h>
#include<smap.h>

/* create a stack copy of a string */
#define STACK_WCSDUP(str, from) \
    wchar_t str[wcslen(from) + 1]; \
    wcscpy(str, from)

/* inform user of fatal error
 * and jump to exception handler */
#define parse_err(state, fmt, ...) \
do { \
    printf("[  error  ] :"); \
    printf(" " fmt "\n", ##__VA_ARGS__); \
    longjmp((state)->err_handler, 1); \
} while(0)

/* warn user of something non-fatal */
#define parse_warn(fmt, ...) \
do { \
    printf("[ warning ] :"); \
    printf(" " fmt "\n", ##__VA_ARGS__); \
} while(0)

/* reserve bytes in memory, returning
 * the location where it was reserved */
#define RESERVE_FUNC(name, type) \
static size_t reserve_ ## name(struct parse_state *state) \
{ \
    size_t ret = state->cur_insns; \
    kprog_append_bytes(state->prog, 0, sizeof(type)); \
    state->cur_insns += sizeof(type); \
    return ret; \
}

/* write bytes to the program memory */
#define WRITE_FUNC(name, type) \
static void write_ ## name(struct parse_state *state, type b, size_t at) \
{ \
    /* TODO unsafe, create kprog -> insert_bytes */ \
    memcpy(state->prog->program + at, &b, sizeof(type)); \
}

/* create implementation of token lookahead buffer */
RBUFF_IMPL(tok_la_buff, struct token, MAX_LOOK);

/* create memory-reserving functions */
RESERVE_FUNC(byte, u8);
RESERVE_FUNC(word, u16);
RESERVE_FUNC(long, u32);

/* create writing functions */
WRITE_FUNC(byte, u8);
WRITE_FUNC(word, u16);
WRITE_FUNC(long, u32);

static const struct token *parse_la(struct parse_state *state);

/* adds a label at the current position
 * returns 1 on redefinition of label */
static int add_label_def(struct parse_state *state, const wchar_t *str, addr_t pos)
{
    if(smap_lookup(state->label_defs, str)){
        return 1;
    }
    struct label_def *def = s_alloc(struct label_def);
    def->pos = pos;
    smap_insert(state->label_defs, str, def);
    return 0;
}

/* adds a label argument to the label argument list */
static void add_label_arg(
        struct parse_state *state,
        const wchar_t *str,
        size_t op,  /* opcode to be changed */
        resolve_f resolver)
{
    struct label_arg *arg = s_alloc(struct label_arg);
    arg->op_pos = op;
    arg->id = wcsdup(str);
    arg->resolve = resolver;
    /* add argument to beginning of list */
    arg->next = state->label_args;
    state->label_args = arg;
}

/* calculates label offets
 * and places them where arguments are needed */
static void resolve_label_arguments(struct parse_state *state)
{
    int err = 0;
    LIST_FREELOOP(struct label_arg, state->label_args, each){
        struct label_def *label = smap_lookup(state->label_defs, each->id);
        if(!label){
            parse_warn("label argument %ls never defined in source", each->id);
            err = 1;
        }else{
            if(each->resolve){
                err = each->resolve(label, each, state);
            }else{
                parse_warn("cannot resolve argument; undefined resolve function");
                err = 1;
            }
        }
        s_free(each->id);
        s_free(each);
    }
    state->label_args = NULL;
    if(err){
        parse_err(state, "unable to resolve label locations due to undefined label");
    }
}

/* Initialize the parser
 * This involves: 
 * - Initializing the lookahead buffer
 * - Initializing the lexer 
 * - Initializing label argument list
 * - Initializing label definition map */
static void parse_init(struct parse_state *state, struct kprog *prog, FILE *f)
{
    tok_la_buff_init(&state->la_buff);
    lex_init(&state->lstate, f, &state->err_handler);

    for(size_t i = 0; i < MAX_LOOK; ++i){
        struct token *t = tok_la_buff_push_back(&state->la_buff);
        lex_next(&state->lstate, t);
    }

    state->prog = prog;
    state->cur_insns = 0;
    state->cur_op = 0;
    state->label_args = NULL;
    state->label_defs = smap_create(s_free);
}

static void parse_destroy(struct parse_state *state)
{
    /* deal with unhandled forward definitions */
    smap_destroy(state->label_defs);
}

/* Test n tokens ahead in the buffer */
static int parse_test_n(struct parse_state *state, size_t n, unsigned tok_type)
{
    if(n >= MAX_LOOK){
        parse_err(state, "exceeded maximum lookahead of %lu", n);
        return 0;
    }
    return tok_la_buff_elem_n(&state->la_buff, n)->type == tok_type;
}

/* test the zeroth token */
static inline int parse_test(struct parse_state *state, unsigned tok_type)
{
    return parse_test_n(state, 0, tok_type);
}

/* test the next 2 tokens */
static inline int parse_test_2(struct parse_state *state, unsigned tok_1, unsigned tok_2)
{
    return parse_test(state, tok_1) && parse_test_n(state, 1, tok_2);
}

/* advance to next token in buffer */
static void parse_advance(struct parse_state *state)
{
    /* delete front element from buffer */
    tok_la_buff_pop_front(&state->la_buff);
    lex_next(&state->lstate, tok_la_buff_push_back(&state->la_buff));
}

/* grab the Nth token */
static inline const struct token *parse_la_n(struct parse_state *state, size_t n)
{
    return tok_la_buff_elem_n(&state->la_buff, n);
}

/* grab the zeroth token */
static inline const struct token *parse_la(struct parse_state *state)
{
    return parse_la_n(state, 0);
}

/* test the next token for a type,
 * issuing an error if it doesnt match */
static void parse_match(struct parse_state *state, unsigned tok_type)
{
    if(parse_la(state)->type != tok_type){
        parse_err(state, "couldn't match target type on \"%ls\"",
                parse_la(state)->lexeme);
    }else{
        parse_advance(state);
    }
}

/* parse a label from the source
 * and insert it into the list of
 * labels to be resolved later */
static int parse_label(struct parse_state *state)
{
    if(!parse_test_2(state, TOK_ID, TOK_COLON)){
        parse_err(state, "mangled label");
    }
    STACK_WCSDUP(label, parse_la(state)->lexeme);
    /* see if this is the entry point */
    if(!wcscmp(label, KPROG_ENTRY_POINT)){
        if(state->prog->entry_point >= 0){
            /* someone redefined the entry point, oh brother */
            parse_err(state, "multiple definitions of entry point");
        }else{
            state->prog->entry_point = state->cur_insns;
        }
    }
    /* make sure we don't already have this label defined */
    if(add_label_def(state, label, state->cur_insns)){
        parse_err(state, "redefinition of label %ls", label);
    }
    parse_match(state, TOK_ID);
    parse_match(state, TOK_COLON);
    return 0;
}

/* find an opcode from a string */
static inline const struct op_def *find_def(const wchar_t *wcs)
{
    for(size_t i = 0; op_defs[i].mnemonic; ++i){
        if(!wcscmp(op_defs[i].mnemonic, wcs)){
            return &op_defs[i];
        }
    }
    return NULL;
}

/* these resolve functions handle labels */

/* resolve an ABCx opcode */
static inline int resolve_abcx(const struct label_def *def,
                        struct label_arg *arg,
                        struct parse_state *state)
{
    ((union opcode *)&state->prog->program[arg->op_pos])->i.Cx = def->pos;
    return 0;
}

/* resolve an Ax opcode */
static inline int resolve_ax(const struct label_def *def,
                      struct label_arg *arg,
                      struct parse_state *state)
{
    ((union opcode *)&state->prog->program[arg->op_pos])->b.Ax = def->pos;
    return 0;
}

/* TODO better error checking */
static long long parse_arg(struct parse_state *state, u8 argmode, u8 which)
{
    const struct token *tok = parse_la(state);
    if(!HAS_ARG(argmode, which)){
        return 0;
    }
    long ret = 0;
    switch(tok->type){
    case TOK_REG:
    case TOK_ADDR:
    case TOK_NUM:
        if((uint16_t)tok->data != tok->data){
            parse_err(state, "\"%ls\" exceeds 16-bit number limit", tok->lexeme);
        }
        ret = tok->data;
        break;
    /* label */
    case TOK_ID:
    {
        u8 mode = GETMODE(argmode);
        switch(mode){
        case iABCx:
            add_label_arg(state, tok->lexeme, state->cur_op, resolve_abcx);
            break;
        case iAx:
            add_label_arg(state, tok->lexeme, state->cur_op, resolve_ax);
            break;
        default:
            parse_err(state, "Address cannot fit into argument");
        }
    
    }
        break;
    default:
        parse_err(state, "expected arg, got \"%ls\"", tok->lexeme);
    };
    parse_advance(state);
    if(parse_test(state, TOK_COMMA)){
        parse_advance(state);
    }
    return ret;
}

static int parse_ins(struct parse_state *state)
{
    const struct token la = *parse_la(state);
    parse_match(state, TOK_ID);
    STACK_WCSDUP(op_str, la.lexeme);
    /* TODO re-implement opcode parsing */
    const struct op_def *op = find_def(op_str);
    if(op){
        union opcode out_op;
        out_op.I = op->code;
        u8 argmode = op->argmode;
        switch(GETMODE(argmode)){
        case iNNNN: /* no arguments */
            break;
        /* TODO these three cases should be simplified
         * into some sort of table lookup */
        case iABCDF:
            out_op.r.A = parse_arg(state, argmode, A_ARG);
            out_op.r.B = parse_arg(state, argmode, B_ARG);
            out_op.r.C = parse_arg(state, argmode, C_ARG);
            out_op.r.D = parse_arg(state, argmode, D_ARG);
            out_op.r.F = parse_arg(state, argmode, F_ARG);
            break;
        case iABCx:
            /* TODO check for argument types */
            out_op.i.A = parse_arg(state, argmode, A_ARG);
            out_op.i.B = parse_arg(state, argmode, B_ARG);
            out_op.i.Cx = parse_arg(state, argmode, Cx_ARG);
            break;
        case iAx:
            out_op.b.Ax = parse_arg(state, argmode, Ax_ARG);
            break;
        };
        write_long(state, out_op.op, reserve_long(state));
        state->cur_op += sizeof(u32);
    }else{
        parse_err(state, "unrecognized opcode \"%ls\" on line %u", op_str, la.line_no);
        return 1;
    }
    return 0;
}

/* parse a single valid line of assembly */
static int parse_expr(struct parse_state *state)
{
    switch(parse_la(state)->type){
    case TOK_ID:
        if(parse_test_n(state, 1, TOK_COLON)){
            if(parse_label(state)){
                parse_err(state, "couldn't parse label");
            }
            return parse_expr(state);
        }else{
            if(parse_ins(state)){
                parse_err(state, "couldn't parse instruction");
            }
            /* only one instruction allowed per line */
            parse_match(state, TOK_EOL);
            return 0;
        }
    case TOK_EOL:
        parse_advance(state);
        return 0;
    default:
        return 1;
    };
}

/* Parse a file into bytecode,
 * placing in the kprog specified */
int parse_file(FILE *in_f, struct kprog *res_prog)
{
    err_on(!in_f, "input file not opened");
    err_on(!res_prog, "output program not allocated");

    struct parse_state state;
    parse_init(&state, res_prog, in_f);

    int ret = 0;
    /* set up error handling */
    if(!setjmp(state.err_handler)){
        const struct token *la = NULL;
        while(!ret && (la = parse_la(&state))->type != TOK_EOS){
            ret = parse_expr(&state);
        }
        resolve_label_arguments(&state);
        if(res_prog->entry_point < 0){
            parse_err(&state, "entry point not defined");
        }
        kprog_finalize(res_prog);
    }else{  /* error specific code */
        ret = 1;
        LIST_FREELOOP(struct label_arg, state.label_args, each){
            s_free(each->id);
            s_free(each);
        }
    }
    parse_destroy(&state);
    return ret;
}
