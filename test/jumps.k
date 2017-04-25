; testing of calculated jumps
__start:
    loadk r0, one
    loadk r1, two
    subs r2, r1, r0
    adds r0, r0, r2
    jmpr r0
one:
    prntv #1
two:
    prntv #2
three:
    prntv #3
four:
    prntv #4
five:
    prntv #5
