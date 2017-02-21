__start:
	loadk r0, #0
	loadk r1, #20
__loop:
	prntr r0
	addiu r0, r0, #1
	bne r0, r1, __loop
