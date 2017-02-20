__start:
	prntv #$44
	loadk r0, #8
	loadk r1, #0
__loop:
	prntr r0
	bne r0, r1, __loop
