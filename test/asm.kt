__start:
	load.b #$5F, L2
	sout.l r1, L1
L2:
	sout.l #0, L1
	nop
L1: L3: L4:
	halt
