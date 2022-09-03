# UCX/OS - Microcontroller Executive / OS

UCX/OS is an experimental preemptive unikernel (library OS) for microcontrollers, aimed to be easily ported. The kernel implements a lightweight multitasking environment in a single address space (based on fibers/coroutines and standard setjmp() and longjmp() library calls), using a minimum amount of resources.

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

The programming model is very simple and intented to be generic for the development of embedded applications. Along with basic C library support, task control and synchronization abstractions are provided. A thin layer of software (HAL, shorthand for *hardware abstraction layer*) is used to generalize basic architecture abstractions, so applications can be compiled for any of the supported targets without change. Any specific functionality besides basic kernel abstractions can also be used, as long as supported by the target architecture and toolchain (for example, abstractions such as port access, timers and other peripherals provided for the AVR target in the AVR-LIBC library). Such additional functionalities are target dependent and their use limits application portability.

### Tasks, the stack, setjmp() / longjmp()

Tasks are basic resources managed by the kernel. In this model, tasks are lightweight execution routines that run indefinitely (tasks never finish) and share the same stack area. During bootup, the kernel initializes all tasks recursively and allocates stack memory for each task local storage. As the same stack is shared by all tasks, context switches are easily implemented by portable setjmp() and longjmp() library calls. This ensures very fast context switches, as less state storage is needed for each task and also allows the kernel to run even on severely memory constrained architectures.

### Scheduling (cooperative / preemptive)

There are two scheduling modes in the kernel. An application can invoke the scheduler cooperatively by making a call to the *ucx_task_yield()* function. After initialization, this can happen at any moment inside the task loop. In preemptive mode, the kernel invokes the scheduler asynchronously using a periodic interrupt. Selection of the scheduling mode is performed according to the return value of the application *app_main()* function. When the application returns from this function with a value of 0, the kernel is configured in cooperative mode. If a value of 1 is returned, the kernel is configured in preemptive mode.

A priority round-robin algorithm performs the scheduling of tasks. By default, all tasks are configured with the same priority (TASK_NORMAL_PRIO), thus tasks share processor time proportionally. Priorities of each task can be changed after their inclusion in the system (in the *app_main()* function) by the *ucx_task_priority()* function, or configured dynamically (inside the body / during execution of a task) using the same function, according to the application needs. Each task can be configured in one of the following priorities: TASK_CRIT_PRIO (critical), TASK_HIGH_PRIO (high), TASK_NORMAL_PRIO (normal), TASK_LOW_PRIO (low) and TASK_IDLE_PRIO (lowest).

### Stack allocation

Memory used for stack inside a task function is allocated from a global stack and divided in two parts. The first part is generally used for task data structures and local task variables, and it is allocated during the first execution of a task. The second part, also known as *guard space*, is allocated after task initialization (after a call to ucx_task_init()). The size of this region is specified when a task is added so it can't be changed. During execution, the guard space will be used for dynamic stack allocation during function calls, temporary variables and also to keep processor state during interrupts.

Each architecture HAL defines a default value for the guard space in a macro (DEFAULT_GUARD_SIZE). Memory constrained architectures, such as the ATMEGA328p have a very limited default guard space of 256 bytes, but other architectures have more (2kB for example). Different tasks may have different guard space sizes. It is up to the user to specify such value according to the application needs.

### Task synchronization (pipes, semaphores)

(TODO)

### Kernel API

* System calls (implemented as library calls)

| Task			| Semaphore		| Pipe			| List / Queue		| LibC			| Utils			|
| :-------------------- | :-------------------- | :-------------------- | :-------------------- | :-------------------- | :-------------------- |
| ucx_task_add()*	| ucx_sem_create()*	| ucx_pipe_create()*	| ucx_list_create()	| ucx_strcpy()		| ucx_printhex()	|
| ucx_task_init()*	| ucx_sem_destroy()*	| ucx_pipe_destroy()*	| ucx_list_destroy()	| ucx_strncpy()		| ucx_hexdump()		|
| ucx_task_yield()*	| ucx_wait()*		| ucx_pipe_flush()*	| ucx_list_add()	| ucx_strcat()		|			|
| ucx_task_delay()*	| ucx_signal()*		| ucx_pipe_size()*	| ucx_list_peek()	| ucx_strncat()		|			|
| ucx_task_suspend()*	|			| ucx_pipe_get()*	| ucx_list_poke()	| ucx_strcmp()		|			|
| ucx_task_resume()*	|			| ucx_pipe_put()*	| ucx_list_count()	| ucx_strncmp()		|			|
| ucx_task_priority()*	|			| ucx_pipe_read()*	| ucx_list_insert()	| ucx_strstr()		|			|
| ucx_task_id()*	|			| ucx_pipe_write()*	| ucx_list_remove()	| ucx_strlen()		|			|
| ucx_task_wfi()*	|			|			| ucx_queue_create()	| ucx_strchr()		|			|
| ucx_task_count()*	|			|			| ucx_queue_destroy()	| ucx_strpbrk()		|			|
| 			|			|			| ucx_queue_count()	| ucx_strsep()		|			|
| 			|			|			| ucx_queue_enqueue()	| ucx_strtok()		|			|
| 			|			|			| ucx_queue_dequeue()	| ucx_strtol()		|			|
| 			|			|			| ucx_queue_peek()	| ucx_memcpy()		|			|
| 			|			|			|			| ucx_memmove()		|			|
| 			|			|			|			| ucx_memcmp()		|			|
| 			|			|			|			| ucx_memset()		|			|
| 			|			|			|			| ucx_abs()		|			|
| 			|			|			|			| ucx_random()		|			|
| 			|			|			|			| ucx_srand()		|			|
| 			|			|			|			| ucx_puts()		|			|
| 			|			|			|			| ucx_gets()		|			|
| 			|			|			|			| ucx_getline()		|			|
| 			|			|			|			| ucx_vsprintf()	|			|
| 			|			|			|			| ucx_printf()		|			|
| 			|			|			|			| ucx_sprintf()		|			|
| 			|			|			|			| ucx_free()*		|			|
| 			|			|			|			| ucx_malloc()*		|			|
| 			|			|			|			| ucx_calloc()*		|			|
| 			|			|			|			| ucx_realloc()*	|			|

#### Task

##### ucx_task_add()

- *Parameters: void \*task, uint16_t guard_size. Returns: int32_t (0, success or -1, fail).* Adds an application task to the system with a TASK_STOPPED state. *\*task* is a pointer to a task function and *guard_size* is a stack reservation amount for recursion and dynamic allocation during task execution. *guard_size* does not include the amount of memory used for local storage allocation (which is automatically allocated in the stack). This function is called during system initialization inside *app_main* and should not be called inside a task. 

##### ucx_task_init()

- *Parameters: none. Returns: nothing.* Initializes a task for execution, putting it in the TASK_READY or TASK_RUNNING state. This function is called once inside a task, after local storage declaration and before the main task loop and should not be called outside the scope of a task.

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

#### List / Queue

(TODO)

#### LibC

- Macros / library abstractions. The internal libc implementation can be bypassed (with function granularity), removing macros thus supporting an external or more adequate implementation.

#### Utils

(TODO)
