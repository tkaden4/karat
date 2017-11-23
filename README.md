# Karat

A simple virtual machine.

This project has two major goals. The first is to write a small OS on top of Karat, 
and the other is to write a scripting language that compiles to karat. Because
of these goals, Karat will try to remain more like an assembly interpreter than
a high-level bytecode interpreter (not the JVM, Python VM, etc.).

## Usage ##
`man ./karat.1` for more details

## System specificiation (In Progress) ##
- Little Endian

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

## In Progress / TODO ##
- [ ] move to 32/64 bit
- [ ] remove lua script
- [ ] documentation
- [ ] embedding library
- [ ] macro system (aliases, functions, loops)
- [ ] typed opcode arguments
- [ ] opcode indirections (simulated CISC)
- [ ] string literals
- [ ] interrupts/ports
- [ ] memory mapped io
- [ ] interrupt vectors
- [ ] jit compilation
- [x] debugger
- [x] modules
- [x] stack + stack opcodes
- [x] command line arguments

## Contribution ##
Karat is open to outside contributions. Look at the above list
to see what needs to be worked on. If you encounter a bug during
development, make an issue or open a pull request with a solution.
Any help is appreciated!
