; print numbers 0-5 using a loop
__start:
    loadk r0, #0
    loadk r1, #5
loop:
    addiu r0, r0, #1
    putr r0
    blt r0, r1, loop
