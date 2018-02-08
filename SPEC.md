# Karat Virtual Machine Specification

This specification is a *work in progress*, and more information
will be added and changed as time goes on and features are added.

## Endianess
Karat uses little-endian storage and addressing, meaning
that the least-significant bytes are placed first and a
memory  addresss. For example, the 32-bit value `0xDEADBEEF`
would be stored in memory location `0x0000` as such:

```asm
0x0000 : 0xEF
0x0001 : 0xBE
0x0002 : 0xAD
0x0003 : 0xDE
```

For a big-endian design, we would just reverse the order.

## Address Space
The address space of Karat extends a 16-bit range, from
0x0000 to 0xffff. Eventually this may be extended to cover
a larger range, such as 32-bits. In order to not overwork 
the memory capabilities of the system being run on, paging
and virtual memory must first be implemented. The lack of 
these features is the basis for the 16-bit address range.
By "overwork", I mean using more RAM than necessary. A
32 bit address space requires 4GB of virtual memory, and

## I/O Ports and Plugins
I/O ports act as the connecting bridge between the central
Karat VM and potential plugins, allowing for extensible 
configurations and new functionality. Conventionally, a
plugin represents a device connected to the computer, such
as a terminal, a network card, or a VGA buffer. Plugins
have I/O ports that can be read from and written to via the 
`get` and `put` instructions.

## Memory Mapped IO and Buffers
MM-IO and MM-Buffers may offer greater succinctity
in communicating with plugins, as well as performance improvements.
For example, the VGA plugin would be better written if the entire
buffer was in main memory, rather than being a separate chunk
maintained by the plugin.

## Exceptions/Interrupts
Exceptions are the result of invalid VM states or operations,
such as a divide by 0 or an invalid opcode in the program.
For example, a division by zero will cause a EXN\_ZERO
exception to be triggered, which will cause a user-defined
handler to be executed, and upon returning will resume execution.
A plugin can cause an interrupt, such as a terminal receiving a
keypress or a network card receiving a packet. Just like exceptions,
a user defined handler will be executed, then normal execution
resumed More abstractly, an interrupt represents an external
state change that requires immediate acknowledgement by the virtual
machine.

## Instruction Set
Karat uses a RISC instruction set, meaning that instructions
are of fixed size: 4 bytes. As of now, 2^6 (64) different
opcodes are possible, but switching to a CISC opcode design
or increasing the opcode size to 8 bytes can be done easily to
solve the problem. Opcodes are of the following forms, inspired
by the Lua 5 opcode design.

- ABCDF: 5 register arguments
- ABCx: 2 register arguments and a 16-bit immediate value
- Ax: 1 26-bit immediate value
- N: No arguments (halt)

The distinction of forms is prevalent due to easy maintenance and
decoding. This way only 4 forms need to be handled by the VM, and 
assemblers don't need to use huge tables of information to determine
the layout of a single instruction.

## KEF/KLF

### KEF - Karat Executable Format
// TODO

### KLF - Karat Linker Format
// TODO
