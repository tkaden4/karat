__start:
_loop:
    loadk r1, #2
    loadr r0, r1
    bne r0, r1, _loop
    prntr r0
    prntr r1
