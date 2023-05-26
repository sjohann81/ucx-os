# UCX/OS - Microcontroller Executive / OS

UCX/OS is an experimental preemptive unikernel (library OS) for microcontrollers, aimed to be easily ported. The kernel implements a lightweight multitasking environment in a single address space (based on fibers/coroutines and standard setjmp() and longjmp() library calls), using a minimum amount of resources.

Currently, UCX/OS supports the following targets:

#### RISC-V (32 / 64 bit)
- RV32I (Qemu)
- RV64I (Qemu)
- HF-RISCV

#### ARM
- Versatilepb
- STM32F4xx (Blackpill)

#### MIPS (32 bit)
- HF-RISC

#### AVR (8 bit)
- ATMEGA328p
- ATMEGA32
- ATMEGA2560


## Supported toolchains

Different toolchains based on GCC and LLVM can be used to build the kernel and applications. If you want to build a cross-compiler from scratch, check the *sjohann81/toolchains* repository for build scripts.


## Features

- Small footprint (4kB ~ 8kB) for the kernel;
- Lightweight task model (fibers) where tasks share the same memory region;
- Preemptive / cooperative scheduling based on a priority round robin (RR) scheduler;
- Task synchronization using semaphores or pipeline channels;
- Dynamic memory allocation;
- Small C library, along with queue and list libraries.


## Building example applications

In order to build the examples, a cross compiler toolchain has to be installed in the build machine. This toolchain will be used to assemble and compile the kernel sources, as well as user application sources and to build a single binary image that can be uploaded to a board or run in a simulator. The build process will generate several files in the *build/target* directory. In order to simplify recurrent application builds, the OS is compiled into a static library (libucxos.a) and application objects are linked with this library in the final build stage.

To build an application, select a supported target and build the kernel. Then, select an application and build it. There is no need to rebuild the kernel if the same chosen target is used, only application sources need to be compiled. For example, to build the '*hello_preempt.c*' application for the Versatilepb target, type '*make ucx ARCH=arm/versatilepb*' to build the kernel and '*make hello_p*' to build the application and link it with the OS library. Check the *makefile* for different rules to build and run the examples. To build applications on a different target, type '*make veryclean*' before rebuilding the kernel again.

For example, to build an application to run on Qemu (32 bit RISC-V) architecture:

- Verify if both a toolchain for RISC-V (riscv32-unknown-elf) and Qemu (qemu-system-riscv32) are installed;
- Build the UCX/OS kernel for the target (*make ucx ARCH=riscv/riscv32-qemu*);
- Build the application (*make hello_p*);
- Run the application (*make run_riscv32*') and type 'Ctrl+a x' to quit the emulator;

For other emulators, the binary image may need to be passed as a parameter as there are no rules in the *makefile* to run the application in this case. For boards such as the Arduino Nano (ATMEGA328p), the binary can be uploaded via a serial port. In the last case, plug the board, check the created virtual serial interface name in */dev/* and verify if the *SERIAL_DEVICE* variable is configured accordingly. To upload the binary to the board, type *make load*.


## Programming model

The programming model is very simple and intented to be generic for the development of embedded applications. Along with basic C library support, task control and synchronization abstractions are provided. A thin layer of software (HAL, shorthand for *hardware abstraction layer*) is used to generalize basic architecture abstractions, so applications can be compiled for any of the supported targets without change. Any specific functionality besides basic kernel abstractions can also be used, as long as supported by the target architecture and toolchain (for example, abstractions such as port access, timers and other peripherals provided for the AVR target in the AVR-LIBC library). Such additional functionalities are target dependent and their use limits application portability.

### Tasks, the stack, setjmp() / longjmp()

Tasks are basic resources managed by the kernel. In this model, tasks are lightweight execution routines that run indefinitely (tasks never finish) and share the same memory region. During bootup, the kernel initializes all tasks and allocates stack memory for each task local storage. Context switches are easily implemented by portable setjmp() and longjmp() library calls. This ensures very fast context switches, as less state storage is needed for each task and also allows the kernel to run even on severely memory constrained architectures.

### Scheduling (cooperative / preemptive)

There are two scheduling modes in the kernel. An application can invoke the scheduler cooperatively by making a call to the *ucx_task_yield()* function. After initialization, this can happen at any moment inside the task loop. In preemptive mode, the kernel invokes the scheduler asynchronously using a periodic interrupt. Selection of the scheduling mode is performed according to the return value of the application *app_main()* function. When the application returns from this function with a value of 0, the kernel is configured in cooperative mode. If a value of 1 is returned, the kernel is configured in preemptive mode.

A priority round-robin algorithm performs the scheduling of tasks. By default, all tasks are configured with the same priority (TASK_NORMAL_PRIO), thus tasks share processor time proportionally. Priorities of each task can be changed after their inclusion in the system (in the *app_main()* function) by the *ucx_task_priority()* function, or configured dynamically (inside the body / during execution of a task) using the same function, according to the application needs. Each task can be configured in one of the following priorities: TASK_CRIT_PRIO (critical), TASK_HIGH_PRIO (high), TASK_NORMAL_PRIO (normal), TASK_LOW_PRIO (low) and TASK_IDLE_PRIO (lowest).

### Stack allocation

Memory used for stack inside a task function is allocated from the heap. The *heap* is a region of memory that is managed by a memory allocator, which is used by both the kernel and applications. Data stored in the task stack is consisted by local task variables and data structures. The size of the stack is configurable per a task basis and is specified when a task is added to the system. During execution, the stack space will be used for dynamic allocation during function calls, temporary variables and also to keep processor state during interrupts.

Each architecture HAL defines a default value for the stack space in a macro (DEFAULT_STACK_SIZE). Memory constrained architectures, such as the ATMEGA328p have a very limited default stack space of 256 bytes, but other architectures have more (2kB for example). Different tasks may have different stack space sizes, and it is up to the user to specify such value according to the application needs.

### Task synchronization (pipes, semaphores)

In real world applications, tasks of the same application have some kind of interaction (synchronization / communication). To support this behavior, two basic abstractions are implemented in the kernel - *pipelines* and *semaphores*. Pipes are character oriented communication channels and semaphores are traditional counting semaphores with atomic semantics.


## APIs

### Kernel API

System calls are divided in three classes. The *task* class of system calls are used for task control and information. The *semaphore* class of system calls are used for task synchronization and the *pipe* class of system calls are used as a basic communication mechanism between tasks. At this moment, system calls are implemented as simple library calls, but this will change in the near future for architectures that suport hardware exceptions and different modes of operation.

| Task			| Semaphore		| Pipe			|
| :-------------------- | :-------------------- | :-------------------- |
| ucx_task_add()	| ucx_sem_create()	| ucx_pipe_create()	|
| ucx_task_yield()	| ucx_sem_destroy()	| ucx_pipe_destroy()	|
| ucx_task_delay()	| ucx_wait()		| ucx_pipe_flush()	|
| ucx_task_suspend()	| ucx_signal()		| ucx_pipe_size()	|
| ucx_task_resume()	|			| ucx_pipe_get()	|
| ucx_task_priority()	|			| ucx_pipe_put()	|
| ucx_task_id()		|			| ucx_pipe_read()	|
| ucx_task_wfi()	|			| ucx_pipe_write()	|
| ucx_task_count()	|			|			|


#### Task

##### ucx_task_add()

- *Parameters: void \*task, uint16_t stack_size. Returns: int32_t (0, success or -1, fail).* Adds an application task to the system with a TASK_STOPPED state. *\*task* is a pointer to a task function and *stack_size* is a stack reservation amount in the heap for recursion and dynamic allocation during task execution and for local storage allocation (which is automatically allocated in the stack). This function is called during system initialization inside *app_main*. 

##### ucx_task_yield()

- *Parameters: none. Returns: nothing.* Yields que processor voluntarily (non-preemptive task reschedule), changing its state to TASK_READY. A task invoking this function gives up execution and calls the scheduler. As a consequence, it is rescheduled to run again in the future.

##### ucx_task_delay()

- *Parameters: uint16_t ticks. Returns: nothing.* Puts the current task in a blocked state changing its state to TASK_BLOCKED for a number of ticks (scheduling events). After the delay, the task state is changed to TASK_READY. If the system is initialized as preemptive, the delay is updated on dispatcher interrupts. Otherwise, *ucx_task_yield()* updates the delay.

##### ucx_task_suspend()

##### ucx_task_resume()

##### ucx_task_priority()

##### ucx_task_id()

##### ucx_task_wfi()

##### ucx_task_count()

#### Semaphore

(TODO)

#### Pipe

(TODO)


### Library API

Lists and queues are basic data structures which are provided to applications as an API. Lists are simply linked lists with automatic memory management. Queues are circular data structures and have a defined size on their creation aligned to the next power of two. This results in an efficient implementation of circular queues, as no modular arithmetic needs to be performed for insertion and removal of items.

| List 			| Queue			|
| :-------------------- | :-------------------- |
| ucx_list_create()	| ucx_queue_create()	|
| ucx_list_destroy()	| ucx_queue_destroy()	|
| ucx_list_add()	| ucx_queue_count()	|
| ucx_list_peek()	| ucx_queue_enqueue()	|
| ucx_list_poke()	| ucx_queue_dequeue()	|
| ucx_list_count()	| ucx_queue_peek()	|
| ucx_list_insert()	| 			|
| ucx_list_remove()	| 			|

#### List

(TODO)

#### Queue

(TODO)

### LibC

Below is a table of the implemented functions from the standard C library. These functions are macros which are used as aliases for actual library abstractions. The internal C library implementation is limited and can be bypassed if needed (with function granularity) by removing such macros from the HAL of a specific architecture, thus supporting an external or more adequate implementation. Functions such as *printf()* are limited implementations which focus on basic functionality, being more compact (smaller code size) versions of the original primitives.

| Libc		|		| 		| 		| 		|
| :------------ | :------------ | :------------ | :------------ | :------------ |
| strcpy()	| strncpy()	| strcat()	| strncat()	| strcmp()	|
| strncmp()	| strstr()	| strlen()	| strchr()	| strpbrk()	|
| strsep()	| strtok()	| strtol()	| memcpy()	| memmove()	|
| memcmp()	| memset()	| abs()		| random()	| srand()	|
| puts()	| gets()	| getline()	| vsprintf()	| printf()	|
| sprintf()	| free()	| malloc()	| calloc()	| realloc()	|
