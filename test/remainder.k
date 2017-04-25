; get the result of 5 % 2 and print 1 - 5 if 1
__start:
    loadk r0, #5
    loadk r1, #2
    modr r2, r0, r1
    loadk r3, #1
    beq r3, r2, equal
    halt
equal:
    prntv #1
    prntv #2
    prntv #3
    prntv #4
    prntv #5
