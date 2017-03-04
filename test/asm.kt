__start:
	loadk r0, #1
	loadk r1, #50
	bne r0, r1, __exit2
	beq r0, r1, __exit
__exit:
	prntv #32
	jmp done
__exit2:
	prntv #22
done:
	prntv #11
