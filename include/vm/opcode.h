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
    const wchar_t * const mnemonic;
    u8 code : 6;
    u8 argmode;
};

#define GETMODE(a) ((a) & 0x3)
/* check to see if opcode uses argument in position */ 
#define HAS_ARG(argm, which) ((argm) & (0x80 >> (which)))

/* convenience struct */
union opcode {
    u32 op;
    union {
        unsigned I: INS_SIZE;
        struct {
            unsigned I: INS_SIZE;
            unsigned A: A_SIZE;
            unsigned B: B_SIZE;
            unsigned C: C_SIZE;
            unsigned D: D_SIZE;
            unsigned F: F_SIZE;
        } r;
        struct {
            unsigned I: INS_SIZE;
            unsigned A: A_SIZE;
            unsigned B: B_SIZE;
            unsigned Cx: Cx_SIZE;
        } i;
        struct {
            unsigned I: INS_SIZE;
            unsigned Ax: Ax_SIZE;
        } b;
    };
};

#define bshl(a, b) (((a) & 0x01) << (b))
#define argmode(a, b, c, d, f, m) \
    (bshl(a, 7) | bshl(b, 6) | bshl(c, 5) | bshl(d, 4) | bshl(f, 3) | (m & 0x3))
#define bmode(ax) argmode(ax, 0, 0, 0, 0, iAx)
#define imode(a, b, cx) argmode(a, b, cx, 0, 0, iABCx)
#define rmode(a, b, c, d, f) argmode(a, b, c, d, f, iABCDF)
#define NO_MODE argmode(0, 0, 0, 0, 0, iNNNN)

#include<opcodes.inc>
