# UCX/OS - Microcontroller Executive / OS

UCX/OS is an experimental preemptive kernel for microcontrollers, aimed to be easily ported. The kernel implements a lightweight multitasking environment (based on fibers/coroutines and standard setjmp() and longjmp() library calls), using a minimum amount of resources.

Currently, the kernel is ported to the following architectures:

### RISC-V (32 / 64 bit)
- RV32I (Qemu)
- RV64I (Qemu)
- HF-RISCV

### MIPS (32 bit)
- HF-RISC

### AVR (8 bit)
- ATMEGA328p

Different toolchains based on GCC can be used to build the kernel and applications. If you want to build a cross-compiler from scratch, check the sjohann81/toolchains repository.
