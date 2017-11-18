print:
    larg r0, #0
    putr r0
    ret

__start:
    pushk #0
    call print
    popr r0
    ret
