; jump using a register
end:
    putv #2
    putv #3
    putv #4
    putv #5
    halt
__start:
    loadk r0, end
    putv #1
    jmpr r0
   
