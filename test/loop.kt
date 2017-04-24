; this program prints numbers 0-5
__start:
    loadk r0, #0
    loadk r1, #5
loop:
    addiu r0, r0, #1
    prntr r0
    bne r0, r1, loop
