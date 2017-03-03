__start:
	loadk r0, #1
	loadk r1, #50
	prntr r0
	prntr r1
	bne r0, r1, __exit
	beq r0, r1, __exit2
__exit:
	prntv #32
	jmp done
__exit2:
	prntv #22
done:
	prntv #11
