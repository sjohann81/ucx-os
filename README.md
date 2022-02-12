# UCX/OS - Microcontroller Executive / OS

UCX/OS is an experimental preemptive kernel for microcontrollers, aimed to be easily ported. The kernel implements a lightweight multitasking environment (based on fibers/coroutines and standard setjmp() and longjmp() library calls), using a minimum amount of resources.

Currently, the kernel supports the following architectures:

#### RISC-V (32 / 64 bit)
- RV32I (Qemu)
- RV64I (Qemu)
- HF-RISCV

#### MIPS (32 bit)
- HF-RISC

#### AVR (8 bit)
- ATMEGA328p
- ATMEGA2560

## Supported toolchains

Different toolchains based on GCC and LLVM can be used to build the kernel and applications. If you want to build a cross-compiler from scratch, check the *sjohann81/toolchains* repository for build scripts.

## Features

- Small footprint (4kB ~ 8kB) for the kernel;
- Lightweight task model (fibers) where tasks share the same stack;
- Preemptive / cooperative scheduling based on a round robin (RR) scheduler;
- Task synchronization using semaphores or pipeline channels;
- Dynamic memory allocation;
- Small LibC, queue and list libraries.

## Building example applications

In order to build the examples, a cross compiler toolchain has to be installed in the host. This toolchain will be used to assemble and compile the kernel sources, as well as the user application and to build a single binary image that can be uploaded to a board or run in a simulator. The build process will generate several files and the final binary is named *image.bin*.

To build an application, uncomment a line that corresponds to the correct target architecture then type *make* followed by the application rule. For example, to build the '*hello_preempt.c*' application, type '*make hello_p*'. Check the makefile for different rules to build and run the examples. To build a different application, type '*make clean*' before the build process command. 

For example, to build an application to run on Qemu (32 bit RISC-V) architecture:

- Verify if a toolchain for RISC-V is installed (riscv32-unknown-elf);
- Verify if Qemu is installed (qemu-system-riscv32);
- Edit the root Makefile, selecting '*ARCH = riscv/riscv32-qemu*' as the target architecture;
- Build the application with '*make hello_p*';
- Run the application with '*make run_riscv32*' (type 'Ctrl+a x' to quit the emulator);

For other emulators, the binary image may need to be passed as a parameter as there are no rules in the makefile to run the application in this case. For boards such as the Arduino Nano (ATMEGA328p), the binary can be uploaded via a serial port. In the last case, plug the board, check the created virtual serial interface name in */dev/* and verify if the *SERIAL_DEVICE* variable is configured accordingly. To upload the binary to the board, type *make load*.

## Programming model

The programming model is very simple and intented to be generic for embedded development. Along with basic C library support, task control and synchronization abstractions are provided. A thin layer of software (HAL) is used to generalize basic architecture abstractions, so applications can be compiled for any of the supported targets without change. Any specific functionality besides basic kernel abstractions can also be used, as long as supported by the target architecture and toolchain (for example, abstractions such as port access, timers and other peripherals provided for the AVR target in the AVR-LIBC library). Such additional functionalities are target dependent and their use limits application portability.

### Tasks, the stack, setjmp() / longjmp()

(TODO)

### Scheduling (cooperative / preemptive)

(TODO)

### Kernel API

(TODO)

#### Task

(TODO)

#### Semaphore

(TODO)

#### Pipe

(TODO)

#### List / Queue

(TODO)

#### LibC

- Macros / library abstractions. The internal libc implementation can be bypassed (with function granularity), removing macros thus supporting an external or more adequate implementation.

### Applications
Call sequence (add tasks, start the kernel, task initialization)

### Stack allocation

Memory used for stack inside a task function is allocated from a global stack and divided in two parts. The first part is generally used for task data structures and local task variables, and it is allocated during the first task execution. The second part, also known as *guard space*, is allocated after task initialization (after a call to ucx_task_init()). The size of this region is specified when a task is added so it can't be changed. During execution, a task will use the guard space for function calls, temporary variables and also to keep processor state during interrupts.

Each architecture HAL defines a default value for the guard space in a macro (DEFAULT_GUARD_SIZE). Memory constrained architectures, such as the ATMEGA328p will use a very limited default guard space of 128 bytes, but other architectures will use more (2kB for example). Different tasks may have different guard space sizes. It is up to the user to specify such value according to the application needs.

### Task synchronization (pipes, semaphores)

(TODO)
