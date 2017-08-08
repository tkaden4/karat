__start:
    loadk r0, #xFFFF
    loadk r1, #0
    loadk r2, #1
loop:
    putr r0
    subs r0, r0, r2 
    bne r0, r1, loop
    loadk r0, #xFFFF
    loadk r1, #0
    loadk r2, #1
loop2:
    putr r0
    subs r0, r0, r2 
    bne r0, r1, loop2
