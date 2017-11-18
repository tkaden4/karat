fib:
    ret

__start:
    loadr r0, #32
    pushr r0
    call fib
    halt
