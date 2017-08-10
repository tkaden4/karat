__start:
    loadk r0, #20
    loadk r1, #40

    pushr r0
    pushr r1

    loadk r0, #44
    loadk r1, #88

    popr r1
    popr r0

    putr r0
    putr r1
    halt
