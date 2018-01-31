/* Simple test programs for unit testing and debugging */

#include<karat/vm/opcode.h>
#include<karat/vm/types.h>

#define r_op(op, a, b, c, d, f) \
    (union opcode){ .r = { .I = op, .A = a, .B = b, .C = c, .D = d, .F = f } }
    
#define i_op(op, _a, _b, _Cx) \
    (union opcode){ .i = { .I = op, .A = _a, .B = _b, .Cx = _Cx } }

#define b_op(op, _Ax) \
    (union opcode){ .b = { .I = op, .Ax = _Ax } }

#define n_op(op) \
    (union opcode){ .I = op }

#define test_prog(program) \
    (struct prog_t){ .bytes = (void *)&program, .size = sizeof(program)*sizeof(union opcode), .entry = 0 }

static union opcode mutual_recursion[] = {
    b_op(CALL_CODE, 8),
    n_op(HALT_CODE),
    // foo:
    b_op(CALL_CODE, 0),
    n_op(RET_CODE)
};

static union opcode  infinite_loop[] = {
    b_op(JMP_CODE, 0)
};

static union opcode write_port[] = {
    i_op(LOADK_CODE, 0, 0, 0),
    i_op(LOADK_CODE, 1, 0, 1),
    i_op(LOADK_CODE, 2, 0, 'A'),
    r_op(PUT_CODE, 0, 1, 2, 0, 0),
    n_op(HALT_CODE)
};
