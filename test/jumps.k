; testing of calculated jumps
__start:
    loadk r0, two
    loadk r1, one
    subs r2, r1, r0
    adds r0, r0, r2
    jmpr r0
zero:
    putv #0
one:
    putv #1
two:
    putv #2
three:
    putv #3
four:
    putv #4
five:
    putv #5
