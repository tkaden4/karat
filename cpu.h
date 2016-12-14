#pragma once

#include<stdint.h>

/* General register */
typedef uint32_t greg_t;
/* Address register */
typedef uint32_t areg_t;
/* address datatype */
typedef uint16_t addr_t;

#define GENERAL_REGS 8	/* number of general registers */
#define ADDRESS_REGS 8	/* number of address registers */

#if GENERAL_REGS < 1
#error
#endif

#if ADDRESS_REGS < 2
#error
#endif

#define SP_INDEX (ADDRESS_REGS - 2)
#define FP_INDEX (ADDRESS_REGS - 1)

#define AREG(cpu, i) cpu.aregs[i]
#define GREG(cput, i) cpu.gregs[i]

#define SP(cpu) AREG(SP_INDEX)
#define FP(cpu)	AREG(FP_INDEX) 

enum {
	NMI = 0,
	IRQ,
	MAX_INTERRUPTS
};

#define INTERRUPT(cpu, i) 

struct cpu_state {
	greg_t gregs[GENERAL_REGS];	/* general registers */
	areg_t aregs[ADDRESS_REGS];	/* address registers */
	addr_t interrupts[MAX_INTERRUPTS];		/* interrupts */
};
