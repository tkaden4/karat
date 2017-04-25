__start:
    loadk r0, #0
    loadk r1, #$2
loop:
    addiu r0, r0, #1
    prntr r0
    blt r0, r1, loop
