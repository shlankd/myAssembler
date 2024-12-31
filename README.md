Assembler Project:
Designed and implemented an assembler for an imaginary assembly language, deepening understanding of machine-level programming and systems architecture.
The project is done with the Linux operating system that runs through VMware Player hypervisor and in C# language.

The imaginary computer and assembly language deffinition:
The processor has 8 general registers, named: r0, r1, r2, r3, r4, r5, r6, r7. The size of each register is 14 bits.
The processor also has a register called PSW (program status word), which contains a number of flags that characterize 
the state of activity at any given moment.
The memory size is 4096 cells, at addresses 0-4095 (in decimal), and each cell is 14 bits in size. 
The use of these flags. A cell in memory is also called a "word". The bits in each word are numbered like in a register.
This computer only works with positive and negative integers. Real numbers are not supported.
Arithmetic is done using the 2's complement method, and there is also support for characters represented in ASCII code.
