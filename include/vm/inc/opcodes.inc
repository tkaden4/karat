/* opcode design
 * i i x x x x x x
 * where i i specifies opcode type (0, 1, or 2 arguments)
 * and x x x x x x specifies opcode
 * due to this, opcodes may only have codes below 0x40
 * This allows for
 * 2^6 (64) CLASS 00 opcodes (zero arguments)
 * 2^6 (64) CLASS 01 opcodes (one argument)
 * 2^6 (64) CLASS 10 opcodes (two arguments)
 * 2^6 (64) CLASS 11 opcodes (experimental)
 * Totaling up to 256 possible opcodes
 * Multi-argument opcodes
 * 
 * */

/* no-argument opcodes */
opcode(L"nop", 0x00, 0)
opcode(L"halt", 0x01, 0)

/* unary opcodes */
opcode(L"goto", 0x11, 1)
opcode(L"prnt", 0x12, 1)
opcode(L"inc", 0x13, 1)

/* binary opcodes */
opcode(L"sout", 0x20, 2)
opcode(L"sin", 0x21, 2)
opcode(L"load", 0x22, 2)
opcode(L"store", 0x23, 2)

