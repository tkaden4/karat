#pragma once

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

#define iABCDF	0x3
#define iABCx	0x2
#define iAx		0x1
#define iNNNN   0x0

#define getargmode()

struct r_opcode {
	unsigned I: 6;
	/* operands */
	unsigned A: 5;
	unsigned B: 5;
	unsigned C: 5;
	unsigned D: 5;
	unsigned F: 6;
};

struct i_opcode {
	unsigned I: 6;
	/* registers */
	unsigned A: 5;
	unsigned B: 5;
	/* immediate value */
	unsigned Cx: 16;
};

struct b_opcode {
	unsigned I: 6;
	/* address */
	unsigned Ax: 26;
};

union opcode {
	unsigned n : 6;
	struct r_opcode r;
	struct i_opcode i;
	struct b_opcode b;
};

struct op_def {
	wchar_t *mnemonic;
	u8 code : 6;
	u8 argmode;
};

#define bshl(a, b) (((a) & 0x01) << (b))

#define argmode(a, b, c, d, f, m) \
	(bshl(a, 7) | bshl(b, 6) | bshl(c, 5) | bshl(d, 4) | bshl(f, 3) | (m & 0x3))

#define bmode(ax) argmode(ax, 0, 0, 0, 0, iAx)
#define imode(a, b, cx) argmode(a, b, cx, 0, 0, iABCx)
#define rmode(a, b, c, d, f) argmode(a, b, c, d, f, iABCDF)

static const struct op_def op_defs[] = {
	/* n opcodes */
	{ L"halt", 0x00, argmode(0, 0, 0, 0, 0, iNNNN) },
	/* r opcodes */
	{ L"adds", 0x10, rmode(1, 1, 1, 0, 0) },	/* signed add */
	{ L"addu", 0x11, rmode(1, 1, 1, 0, 0) },	/* unsigned add */
	/* i opcodes */
	{ L"addsi",	0x20, imode(1, 0, 1) },	/* immediate signed add */
	{ L"addsu",	0x21, imode(1, 0, 1) },	/* immediate unsigned add */
	{ L"loadk", 0x22, imode(1, 0, 1) }, /* load constant into register */
	/* b opcodes */
	{ L"jmp", 0x30, bmode(1) },	/* jump */
	{ L"jal", 0x31, bmode(1) },	/* jump and link */
};
