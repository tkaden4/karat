__start:
    loadk r1, #0
    loadk r0, #$FFFFFFF
loop:
    addiu r1, r1, #1
    prntr r1
    bne r1, r0 loop
