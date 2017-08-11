print:
    putr r0
    ret

__start:
    loadk r0, #1
    call print
    loadk r0, #2
    call print
    loadk r0, #3
    call print
    loadk r0, #4
    call print
    loadk r0, #5
    call print
