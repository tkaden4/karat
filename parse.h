#pragma once

#include"system.h"
#include"cpu.h"
#include"list.h"
#include"opcode.h"

/* TODO add syntax such as:

multiple actions per line 
label: move 0, 0

variable/array syntax
label: db 0, db 0

convenience macros (preprocessor)
#loop(0, 10){
	move r0, r#i
}
and
move r0, r1[+2] which expands to 

add r1, 2
move r0, r1
*/

/* parsed line data */
struct presult {
	SLINK(struct presult);	/* for multi-instruction lines */
	u8 op_flags;	/* operand flags */
	union {
		const wchar_t *label;
		/* constant values */
		addr_t aval;
		i32 ival;
		float fval;
	} args[2];	/* arguments (0 for none) */
};

bool parse_line(const wchar_t *line, struct presult *res);
