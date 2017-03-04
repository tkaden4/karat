#include<vm/opcode.h>

enum {

};

#define bshl(a, b) (((a) & 0x01) << (b))
#define argmode(a, b, c, d, f, m) \
	(bshl(a, 7) | bshl(b, 6) | bshl(c, 5) | bshl(d, 4) | bshl(f, 3) | (m & 0x3))
#define bmode(ax) argmode(ax, 0, 0, 0, 0, iAx)
#define imode(a, b, cx) argmode(a, b, cx, 0, 0, iABCx)
#define rmode(a, b, c, d, f) argmode(a, b, c, d, f, iABCDF)
#define NO_MODE argmode(0, 0, 0, 0, 0, iNNNN)

const struct op_def op_defs[MAX_OPCODES] = {
	/* n opcodes */
	{ L"halt", 0x00, NO_MODE },
	/* r opcodes */
	{ L"adds", 0x10, rmode(1, 1, 1, 0, 0) },	/* signed add */
	{ L"addu", 0x11, rmode(1, 1, 1, 0, 0) },	/* unsigned add */
	{ L"xor", 0x12, rmode(1, 1, 1, 0, 0) },		/* xor two registers */
	/* i opcodes */
	{ L"addis",	0x20, imode(1, 1, 1) },	/* immediate signed add */
	{ L"addiu",	0x21, imode(1, 1, 1) },	/* immediate unsigned add */
	{ L"loadk", 0x22, imode(1, 0, 1) }, /* load constant into register */
	{ L"putb", 0x23, imode(1, 1, 1) },	/* put byte */
	{ L"putw", 0x24, imode(1, 1, 1) },	/* put word */
	{ L"putl", 0x25, imode(1, 1, 1) },	/* put long */
	{ L"getb", 0x26, imode(1, 1, 1) },	/* get byte */
	{ L"getw", 0x27, imode(1, 1, 1) },	/* get word */
	{ L"getl", 0x28, imode(1, 1, 1) },	/* get long */
	{ L"prntr", 0x29, imode(1, 0, 0) },	/* print register */
	{ L"prntv", 0x2A, imode(0, 0, 1) },	/* print value */
	{ L"beq", 0x2B, imode(1, 1, 1) },	/* branch on equal */
	{ L"bne", 0x2C, imode(1, 1, 1) },	/* branch on not equal */
	{ L"jmp", 0x2D, bmode(1) },	/* jump */
	{ NULL, 0, NO_MODE }
};
