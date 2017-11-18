print_stack:
    popr r1
    popr r0
    putr r0
    pushr r1
    ret

__start:
    pushk #1
    pushk #2
    pushk #3
    pushk #4
    pushk #5
    call print_stack
    call print_stack
    call print_stack
    call print_stack
    call print_stack
