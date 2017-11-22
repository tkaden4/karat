# Karat

A simple virtual machine.

This project has two major goals. The first is to write a small OS on top of Karat, 
and the other is to write a scripting language that compiles to karat. Because
of these goals, Karat will try to remain more like an assembly interpreter than
a high-level bytecode interpreter (not the JVM, Python VM, etc.).

### Building ###
Build entire project:
`make`

Build interpreter:
`make karat`

Build default modules:
`make modules`

### Preprocessor Flags ###
#### KDEBUG ####
Keep debug code in
#### KARAT\_PROD ####
Turn off some safety checks for production

## In Progress ##
- [ ] memory mapped io
- [ ] interrupt vectors
- [ ] documentation
- [ ] remove lua script
- [ ] debugger
- [ ] embedding library
- [ ] macro system (aliases, functions, loops)
- [ ] typed opcode arguments
- [ ] string literals
- [ ] interrupts/ports
- [ ] jit compilation
- [ ] opcode indirections (simulated CISC)
- [x] modules
- [x] stack + stack opcodes
- [x] command line arguments
