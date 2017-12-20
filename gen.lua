-- Generates all opcodes

-- maximum allowed opcodes
local MAX_OPCODES = 64

function opcode(name, number, mode)
    output(string.format("{ L\"%s\", %s, %s }, ", name, number, mode));
end

-- create indented output
local indent = 0
function output(str)
    print(string.rep("    ", indent) .. str)
end

-- actual opcodes
local opcodes = {
    { "halt", "NO_MODE" },
    { "ret",  "NO_MODE" },
    { "dup",  "NO_MODE" },
    { "popa", "NO_MODE" },
    { "pusha","NO_MODE" },
    -- R opcodes
    { "adds",  "rmode(1, 1, 1, 0, 0)" }, -- signed add
    { "addu",  "rmode(1, 1, 1, 0, 0)" }, -- unsigned add
    { "subs",  "rmode(1, 1, 1, 0, 0)" }, -- subtract two registers
    { "subu",  "rmode(1, 1, 1, 0, 0)" }, -- subtract two registers
    { "muls",  "rmode(1, 1, 1, 0, 0)" }, -- multiply two registers
    { "mulu",  "rmode(1, 1, 1, 0, 0)" }, -- multiply two registers
    { "divs",  "rmode(1, 1, 1, 0, 0)" }, -- divide two registers
    { "divu",  "rmode(1, 1, 1, 0, 0)" }, -- divide two registers
    { "xorr",  "rmode(1, 1, 1, 0, 0)" }, -- xor two registers
    { "loadr", "rmode(1, 1, 0, 0, 0)" }, -- load register into register
    { "modr",  "rmode(1, 1, 1, 0, 0)" }, -- get remainder
    { "push",  "rmode(1, 0, 0, 0, 0)" }, -- push register onto stack
    { "pop",   "rmode(1, 0, 0, 0, 0)" }, -- pop value into register
    { "trap",  "rmode(1, 0, 0, 0, 0)" }, -- do trap
    { "read",  "rmode(1, 1, 0, 0, 0)" }, -- read value from memory
    { "stor",  "rmode(1, 1, 0, 0, 0)" }, -- store value into memory
    -- I opcodes
    { "jmpr",  "imode(1, 0, 0)" }, -- jump relative to register
    { "pushk", "imode(0, 0, 1)" }, -- push a constant to the stack
    { "addis", "imode(1, 1, 1)" }, -- immediate signed add */
    { "addiu", "imode(1, 1, 1)" }, -- immediate unsigned add */
    { "subis", "imode(1, 1, 1)" }, -- immediate unsigned sub */
    { "subiu", "imode(1, 1, 1)" }, -- immediate unsigned sub */
    { "loadk", "imode(1, 0, 1)" }, -- load constant into register */
    { "putr",  "imode(1, 0, 0)" }, -- print register */
    { "putv",  "imode(0, 0, 1)" }, -- print value */
    { "beq",   "imode(1, 1, 1)" }, -- branch on equal */
    { "bne",   "imode(1, 1, 1)" }, -- branch on not equal */
    { "bgt",   "imode(1, 1, 1)" }, -- branch on not equal */
    { "blt",   "imode(1, 1, 1)" }, -- branch on not equal */
    -- B opcodes
    { "jmp",   "bmode(1)" },       -- jump */
    { "call",  "bmode(1)" },       -- jump */

}

if #opcodes >= MAX_OPCODES then
    print "too many opcodes"
    return 1
end

output("#define MAX_OPCODES " .. MAX_OPCODES)

output("enum {")
indent = indent + 1
local defnames = {}
for i, v in pairs(opcodes) do
    local def = string.format("%s_CODE", string.upper(v[1]))
    defnames[i] = def
    output(string.format("%s = 0x%02X,", def, i - 1));
end
indent = indent - 1
output("};")

print("")

output "static const struct op_def op_defs[MAX_OPCODES] = {"

indent = indent + 1
-- print opcodes
for k, v in pairs(opcodes) do
    opcode(v[1], defnames[k], v[2])
end
-- create sentinel value
output "{ NULL, 0, NO_MODE }, "
indent = indent - 1
output "};"
