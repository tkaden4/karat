__start:
	xor r0, r0
	loadk r1, #20
loop:
	addu r0, r0, #1
	prnt r0
	beq r0, r1, loop
