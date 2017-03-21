#pragma once

#include<wchar.h>
#include<ktypes.h>

/* opcode design
* All opcodes are 32 bits wide (easier to parse)
* and follow the MIPS style, though some changes
* have been made
*
* OPCODE LAYOUT
* I A B C D F
* I A B C Dx_
* I A B Cx___
* I Ax_______
*
* I is the opcode
* A-D are registers
* Ax is a value
* Cx is a value
* F is an extra "scratch" value (as in no use for it yet)
* */

/* TODO fix some flaws in the opcode system
 * that may lead to bugs in the future, such
 * as Cx_ARG being equal to C_ARG */

#define INS_SIZE 6
#define A_SIZE 5
#define B_SIZE 5
#define C_SIZE 5
#define D_SIZE 5
#define F_SIZE 6
#define Dx_SIZE (D_SIZE + F_SIZE)
#define Cx_SIZE (C_SIZE + Dx_SIZE)
#define Ax_SIZE (A_SIZE + B_SIZE + Cx_SIZE)

#define A_POS INS_SIZE
#define B_POS (A_POS + A_SIZE)
#define C_POS (B_POS + B_SIZE)
#define D_POS (C_POS + C_SIZE)
#define F_POS (D_POS + D_SIZE)
#define Ax_POS A_POS
#define Cx_POS C_POS

enum {
    iNNNN = 0,
    iAx,
    iABCx,
    iABCDF,
    ARG_MODES
};

/* argument identifiers */
enum {
    A_ARG = 0,
    B_ARG,
    C_ARG,
    D_ARG,
    F_ARG,
    N_ARG,
    Ax_ARG = A_ARG,
    Cx_ARG = C_ARG,
    Dx_ARG = D_ARG,
};

/* argument types */
enum {
    iArgN = 0,  /* not used */
    iArgU,      /* used, any value */
    iArgK,      /* constant value */
    iArgR,      /* register */
    iArgA,      /* address/label */
};

/* argmode
 * argument types  mode
 * aaabbbcccdddfff mm 
 * 17 bits without */

struct op_def {
    wchar_t *mnemonic;
    u8 code : 6;
    u8 argmode;
};

#define GETMODE(a) ((a) & 0x3)
/* check to see if opcode uses argument in position */ 
#define HAS_ARG(argm, which) ((argm) & (0x80 >> (which)))
//#define ARG_TYPE(argm, which) ((argm >> which) & 0x);

#define MAX_OPCODES (64)
extern const struct op_def op_defs[MAX_OPCODES];

union opcode {
    u32 op;
    union {
        unsigned I: 6;
        struct {
            unsigned I: 6;
            unsigned A: 5;
            unsigned B: 5;
            unsigned C: 5;
            unsigned D: 5;
            unsigned F: 6;
        } r;
        struct {
            unsigned I: 6;
            unsigned A: 5;
            unsigned B: 5;
            unsigned Cx: 16;
        } i;
        struct {
            unsigned I: 6;
            unsigned Ax: 26;
        } b;
    };
};
