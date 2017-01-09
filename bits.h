#pragma once

/* shift i bytes to the left */
#define shl(b, i) ((b) << (8 * (i)))

#define sizeof_bits(type) (sizeof(type)*8)

/* get a the lowest n bits of a number */
#define lbits(num, n) \
	((num << (bs(num) - n)) >> (bs(num) - n))
