__start:
    loadk r0, #5
    loadk r1, #2
    modr r2, r0, r1
    loadk r3, #1
    beq r3, r2, _equal
    halt
_equal:
    loadk r0, #0
    loadk r1, #0
    prntr r2
