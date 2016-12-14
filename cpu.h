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

#define SP_INDEX (ADDRESS_REGS - 2)
#define FP_INDEX (ADDRESS_REGS - 1)

enum {
	NMI = 0,
	IRQ, 
	INTERRUPTS
};

struct cpu_state {
	/* general registers */
	union {
		greg_t gregs[GENERAL_REGS];
		struct {
			greg_t g0;
			greg_t g1;
			greg_t g2;
			greg_t g3;
			greg_t g4;
			greg_t g5;
			greg_t g6;
			greg_t g7;	
		};
	};
	/* address registers */
	union {
		areg_t aregs[ADDRESS_REGS];
		struct {
			areg_t a0;
			areg_t a1;
			areg_t a2;
			areg_t a3;
			areg_t a4;
			areg_t a5;
			areg_t sp;	/* stack pointer */
			areg_t fp;	/* frame pointer */
		};
	};
	/* interrupts */
	union {
		addr_t interrupts[INTERRUPTS];
		struct {
			addr_t nmi;
			addr_t irq;
		};
	};
};
