; get the result of 5 % 2 and print 1 - 5 if 1
__start:
    loadk r0, #5
    loadk r1, #2
    modr r2, r0, r1
    loadk r3, #1
    beq r3, r2, equal
    halt
equal:
    putv #1
    putv #2
    putv #3
    putv #4
    putv #5
