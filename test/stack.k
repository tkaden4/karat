__start:
    loadk r0, #5
    ; push values
loop:
    pushr r0
    subis r0, r0, #1
    loadk r1, #0
    bne r0, r1, loop
    ; pop values
    loadk r0, #5
_loop:
    popr r1
    putr r1
    subis r0, r0, #1
    loadk r1, #0
    bne r0, r1, _loop
