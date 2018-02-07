# Karat Virtual Machine Specification

This specification is a *work in progress*, and more information
will be added and changed as time goes on and features changed.

## Endianness
Karat uses little-endian storage and addressing, meaning
that the least-significant bytes are placed first and a
memory  addresss. For example, the 32-bit value `0xDEADBEEF`
would be stored in memory location `0x0000` as such:

`0x0000 : 0xEF`
`0x0001 : 0xBE`
`0x0002 : 0xAD`
`0x0003 : 0xDE`

## Address Space
The address space of Karat extends a 16-bitf range, from
0x0000 to 0xffff. Eventually this may be extended to cover
a larger range, such as 32-bits. In order to not overwork 
the memory capabilities of the system being run on, paging
and virtual memory must first be implemented. The lack of 
these features is the basis for the 16-bit address range.

## I/O Ports and Plugins
I/O ports act as the connecting bridge between the central
Karat VM and potential plugins, allowing for extensible 
configurations and new functionality. Conventionally, a
plugin represents a device connected to the computer, such
as a terminal, a network card, or a VGA buffer. Plugins
have I/O ports that can be read from and written to via the 
`get` and `put` instructions.

## Memory Mapped IO and Buffers
Currently, IO ports are separate from the memory model, 
occupying their own space. However, MM-IO and MM-Buffers
may offer greater succinctity in communicating with 
plugins, as well as performance improvements. For example,
the VGA plugin would be better written if the entire
buffer was in main memory, rather than being a separate chunk
maintained by the plugin.

## Exceptions/Interrupts
Exceptions/Interrupts both interrupt the current instruction
being processed and immediately call the handler associated
with it. For example, a division by zero will cause a EXN\_ZERO
exception to be triggered, which will cause a user-defined
handler to be executed, and upon returning will resume normal
execution. Interrupts are caused by plugins that need to notify
the VM immediately, such as a terminal receiving a keypress or
a network card receiving a packet. More abstractly, an interrupt
represents an external state change that requires immediate
acknowledgement by the virtual machine.
