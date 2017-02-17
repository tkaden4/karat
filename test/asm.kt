__start:
	load r0, #0
loop:
	prnt r0
	inc r0
	cmp r0, #20
	cmp r0, #20
	bne loop
