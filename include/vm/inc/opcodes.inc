/* opcode design (REFER TO MIPS) */

opcode(L"nop", 0x00, 0)
opcode(L"halt", 0x01, 0)

opcode(L"goto", 0x11, 1)
opcode(L"prnt", 0x12, 1)
opcode(L"inc", 0x13, 1)
opcode(L"dec", 0x14, 1)
opcode(L"beq", 0x15, 1)
opcode(L"bne", 0x16, 1)

opcode(L"sout", 0x20, 2)
opcode(L"sin", 0x21, 2)
opcode(L"load", 0x22, 2)
opcode(L"store", 0x23, 2)
opcode(L"cmp", 0x24, 2)

/* b instructions */
/* r instructions */
/* i instructions */
