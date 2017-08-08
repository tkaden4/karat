__start:
    loadk r0, duff
; calculate result of 5 % 3
    loadk r1, #5
    loadk r2, #3
    modr r1, r1, r2
; calculate offset
    loadk r3, #0
    loadk r4, #4
    ; muls r0, r1, r4
mult:
    adds r0, r0, r1
    addiu r3, r3, #1
    blt r3, r4, mult
; jump to instruction
    jmpr r0
duff:
    putv #1
    putv #2
; it should jump to here
    putv #1
    putv #2
    putv #3
    putv #4
    putv #5
