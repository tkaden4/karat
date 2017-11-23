fib:
    ret

__start:
    loadk r0, #32
    pushr r0
    call fib
    halt
