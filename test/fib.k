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
    addiu sp, sp, #4
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
fib_base_case:
    loadk r2, #1
    stor r2, r0
fib_done:
    popa
    ret
__start:
    addiu sp, sp, #4
    pushk #10
    call fib
    pop r0
    pop r1

    copy r1, sp
    loadk r0, #x61
    pushb r0
    inc r0
    pushb r0
    inc r0
    pushb r0
    inc r0
    pushb r0

    loadk r0, #0
    pushb r0

    trap #0

    subis sp, sp, #5

    halt
