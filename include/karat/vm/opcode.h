#pragma once

#include<wchar.h>
#include<stdint.h>

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
* F is an extra "scratch" value (no current use)
* */

#define MAX_OPCODES 64
#define OPCODE_SIZE 32
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
    iABCDF
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
    iArgK,      /* constant value */
    iArgR,      /* register */
    iArgA,      /* address/label */
};


typedef uint16_t amode_t;

struct op_def {
    const wchar_t * const mnemonic;
    unsigned code : INS_SIZE;
    amode_t argmode;
};

/* argument mode
 * tt tt tt tt tt mm
 * tt is the type for each argument
 * mm is the argument mode */

/* shift a bit left */
#define bshl(a, b) (((a) & 0x01) << (b))
/* create an argument mode */
#define argmode(a, b, c, d, f, m) \
    (bshl(a, 7) | bshl(b, 6) | bshl(c, 5) | bshl(d, 4) | bshl(f, 3) | (m & 0x3))
/* mode for branching instructions */
#define bmode(ax) argmode(ax, iArgN, iArgN, iArgN, iArgN, iAx)
/* mode from immediate instructions */
#define imode(a, b, cx) argmode(a, b, cx, iArgN, iArgN, iABCx)
/* mode from register instructions */
#define rmode(a, b, c, d, f) argmode(a, b, c, d, f, iABCDF)
/* argument with no mode */
#define NO_MODE argmode(iArgN, iArgN, iArgN, iArgN, iArgN, iNNNN)
/* get the argument mode */
#define MODETYPE(a) ((a) & 0b11)
/* check to see if opcode uses argument in position */ 
#define HAS_ARG(argm, which) ((argm) & (0b10000000 >> (which)))

/* convenience struct */
union opcode {
    unsigned op : OPCODE_SIZE;
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

#include<karat/opcodes.inc>
