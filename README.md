# Karat

A virtual computer for hobby development.

## Usage ##
`man ./karat.1` for more details

## System specificiation (In Progress) ##
- Big Endian
- Multi-word instruction set

### Building ###
Build entire project:
`make`

Build VM:
`make karat`

Build default modules:
`make modules`

### Preprocessor Flags ###
#### KDEBUG ####
Keep debug code in
#### KARAT\_PROD ####
Turn off some safety checks for production

## In Progress / TODO ##
- [ ] CISC -> RISC
- [ ] move to 32 bit
- [ ] remove lua script
- [ ] documentation
- [ ] macro system (aliases, functions, loops)
- [ ] typed opcode arguments
- [ ] string literals
- [ ] interrupts/ports
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
