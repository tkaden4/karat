; (i32) -> i32
fib:
    pusha
    subis r0, sp, #128
    ; read 'n' into r1
    read r1, r0
    ; set r0 to result
    subis r0, r0, #4
    ; test for n < 2
    loadk r2, #2
    blt r1, r2, fib_base_case

    ; fib(n - 1)
    subis r1, r1, #1
    pushk #0
    push r1
    call fib
    pop r1
    pop r3

    ; fib(n - 2)
    subis r1, r1, #1
    addiu sp, sp, #4
    push r1
    call fib
    pop r1
    pop r4

    adds r5, r3, r4
    stor r5, r0

    jmp fib_done
    ; less than 2
fib_base_case:
    loadk r2, #1
    stor r2, r0
    ; finished
fib_done:
    popa
    ret

__start:
    addiu sp, sp, #4
    pushk #34
    call fib
    pop r0
    pop r1
    trap #1
    halt
