__start:
    ; previous value
    loadk r1, #0
    ; result
    loadk r2, #1
    ; maximum value
    loadk r3, #47
    ; simple constants
    loadk r4, #1
    loadk r5, #0
loop:
    ; save current value
    loadr r7, r2
    ; add previous result to current result
    adds r2, r2, r1
    ; load old value 
    loadr r1, r7
    ; subtract instance count
    subis r3, r3, #1
    ; exit loop if done iterating
    bne r3, r5, loop
    ; print result
    putr r2
