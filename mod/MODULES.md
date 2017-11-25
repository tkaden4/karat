# Modules

Modules represent devices connected to the VM that can be used
to extend the functionality of Karat beyond what is provided
by the basic assembler. This was done in order to strip Karat
of any bias towards a specific domain.


Modules are loaded at runtime, in order to remove the cost of
recompilation every time you wish to add a new module and also
to allow more flexible configurations.


Examples of modules include:
- Console IO
- Networking support
- Keyboard Support
- Graphics Support


The exact interface to be used from Karat to interact with
a module is under development, but most likely it will 
revolve around interrupts and serial ports.


The module definition interface is under development as 
well, but should allow for flexible module development.
