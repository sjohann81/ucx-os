# UCX/OS - Microcontroller Executive / OS

UCX/OS is an experimental preemptive nanokernel for microcontrollers, aimed to be easily ported. The kernel implements a lightweight multitasking environment in a single address space (based on tasks and coroutines), using a minimum amount of resources.

Currently, UCX/OS supports the following targets:

#### RISC-V (32 / 64 bit)
- RV32I (Qemu)
- RV64I (Qemu)
- HF-RISCV (RV32E / RV32I)

#### ARM (32 bit)
- Versatilepb
- STM32F401 / STM32F411 (Black Pill)
- STM32F407 (Discovery)

#### MIPS (32 bit)
- HF-RISC

#### AVR (8 bit)
- ATMEGA328p
- ATMEGA32
- ATMEGA2560


## Supported toolchains

Different toolchains based on GCC and LLVM can be used to build the kernel and applications. If you want to build a cross-compiler from scratch, check the *sjohann81/toolchains* repository for build scripts.


## Features

- Small footprint (6kB ~ 10kB) for the kernel.
- Hybrid lightweight task model (taks and coroutines) where tasks share the same memory region;
- Preemptive / cooperative scheduling based on a priority round robin (RR) scheduler and a user defined realtime scheduler;
- Task synchronization and communication using semaphores, pipeline channels or message queues;
- Software timers with callback execution;
- Dynamic memory allocation;
- Small C library, along with data structures library.


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

### Tasks, the stack, execution context and coroutines

Tasks are basic resources managed by the kernel. In this model, tasks are lightweight execution routines that run indefinitely (tasks never finish. unless canceled) and share the same memory region. During bootup, the kernel initializes all tasks and allocates stack memory for each task local storage. On most targets, context switches can be easily implemented by portable *setjmp()* and *longjmp()* library calls. This ensures very fast context switches, as less state storage is needed for each task and also allows the kernel to run even on severely memory constrained architectures.

Coroutines are lightweight, stackless scheduling resources. Coroutines can be used in separated groups of coroutines, where each group can be managed by a task. Another way to use coroutines is in an application where no tasks are scheduled and only coroutines are executed directly from the *app_main()* context. Coroutines should always return to yield execution to other coroutines (in contrast to tasks) and are scheduled by the application itself. The same stack is shared by all coroutines in the same group.

### Scheduling (cooperative / preemptive)

There are two task scheduling modes in the kernel. An application can invoke the scheduler cooperatively by making a call to the *ucx_task_yield()* function. After initialization, this can happen at any moment inside the task loop. In preemptive mode, the kernel invokes the scheduler asynchronously using a periodic interrupt. Selection of the scheduling mode is performed according to the return value of the application *app_main()* function. When the application returns from this function with a value of 0, the kernel is configured in cooperative mode. If a value of 1 is returned, the kernel is configured in preemptive mode.

A priority round-robin algorithm performs the scheduling of tasks. By default, all tasks are configured with the same priority (TASK_NORMAL_PRIO), thus tasks share processor time proportionally. Priorities of each task can be changed after their inclusion in the system (in the *app_main()* function) by the *ucx_task_priority()* function, or configured dynamically (inside the body / during execution of a task) using the same function, according to the application needs. Each task can be configured in one of the following priorities: TASK_CRIT_PRIO (critical), TASK_REALTIME_PRIO (real time), TASK_HIGH_PRIO (high), TASK_ABOVE_PRIO (above normal), TASK_NORMAL_PRIO (normal), TASK_BELOW_PRIO (below normal), TASK_LOW_PRIO (low) and TASK_IDLE_PRIO (lowest).

Another scheduling resource are coroutines, which are a lightweight mechanism. Coroutines can run in a standalone manner (without tasks in the system) or within a task context, and they have their own priority based round-robin scheduler.

The task control block also holds a pointer to a user defined (realtime) scheduler. If implemented, this scheduler has a greater priority over the default (best effort) round-robin scheduling policy. Realtime tasks are defined just as normal tasks, but the user has to implement the scheduler, setup a reference to this scheduler in the *kernel control block* and setup task priorities using the *ucx_task_rt_priority()* function.


### Stack allocation

Memory used for stack inside a task function is allocated from the heap. The *heap* is a region of memory that is managed by a memory allocator, which is used by both the kernel and applications. Data stored in the task stack is consisted by local task variables and data structures. The size of the stack is configurable per a task basis and is specified when a task is added to the system. During execution, the stack space will be used for dynamic allocation during function calls, temporary variables and also to keep processor state during interrupts.

Each architecture HAL defines a default value for the stack space in a macro (DEFAULT_STACK_SIZE). Memory constrained architectures, such as the ATMEGA328p have a very limited default stack space of 256 bytes, but other architectures have more (2kB for example). Different tasks may have different stack space sizes, and it is up to the user to specify such value according to the application needs.

### Task synchronization (semaphores, pipes, message queues, event queues)

In real world applications, tasks of the same application have some kind of interaction (synchronization / communication). To support this behavior, two basic abstractions are implemented in the kernel - *pipelines* and *semaphores*. Pipes are character oriented communication channels and semaphores are traditional counting semaphores with atomic semantics.

### Device driver interface

Device drivers are the way to enable portability, customization and hardware support for different targets along with kernel extensions for new functionality. Device drivers can be implemented with a generic interface with operations such as *open()*, *close()*, *read()* and *write()* or with a custom interface. A typical device driver is saparated in three parts and an application can create one or more instances of the same driver.


## APIs

### Kernel API

System calls are divided in several classes. The *task* class of system calls are used for task control and information. The *coroutine* class of system calls implement coroutine grouping and scheduling. The *system* class handle system information and control. The *semaphore* class of system calls are used for task synchronization, along with the *pipe* class which define a basic communication mechanism between tasks and coroutines and the more flexible *message queue*. The *timer* interface define system calls that can be used to create configurable and low overhead timers. At this moment, system calls are implemented as simple library calls, but this will change in the near future for architectures that suport hardware exceptions and different modes of operation. There is a system call wrapper in place that can be used for as a system call interface, which implements a software interrupt for syscalls and asynchronous callbacks.

| Task			| Coroutine		| System		| Semaphore		| Pipe			| Message Queue		| Timer			|
| :-------------------- | :-------------------- | :-------------------- | :-------------------- | :-------------------- | :-------------------- | :-------------------- |
| ucx_task_spawn()	| ucx_cr_ginit()	| ucx_ticks()		| ucx_sem_create()	| ucx_pipe_create()	| ucx_mq_create()	| ucx_timer_create()	|
| ucx_task_cancel()	| ucx_cr_gdestroy()	| ucx_uptime()		| ucx_sem_destroy()	| ucx_pipe_destroy()	| ucx_mq_destroy()	| ucx_timer_destroy()	|
| ucx_task_yield()	| ucx_cr_add()		|			| ucx_sem_wait()	| ucx_pipe_flush()	| ucx_mq_enqueue()	| ucx_timer_start()	|
| ucx_task_delay()	| ucx_cr_cancel()	| 			| ucx_sem_trywait()	| ucx_pipe_size()	| ucx_mq_dequeue()	| ucx_timer_cancel()	|
| ucx_task_suspend()	| ucx_cr_schedule()	|			| ucx_sem_signal()	| ucx_pipe_read()	| ucx_mq_peek()		|			|
| ucx_task_resume()	|			|			| 			| ucx_pipe_write()	| ucx_mq_items()	| 			|
| ucx_task_priority()	|			| 			| 			| ucx_pipe_nbread()	|			|			|
| ucx_task_rt_priority()|			| 			| 			| ucx_pipe_nbwrite()	|			|			|
| ucx_task_id()		|			| 			|			| 			|			|			|
| ucx_task_refid()	|			| 			| 			|			|			|			|
| ucx_task_wfi()	|			|			| 			|			|			|			|
| ucx_task_count()	|			|			| 			|			|			|			|


#### Task

Tasks are the basic scheduling resource. An application in UCX/OS is composed of one or more tasks, which are scheduled according to their priorities. Tasks communicate using shared memory and are synchronized by exchanging data through pipes (using blocking and non-blocking calls) or event queues.

##### ucx_task_spawn()

- Spawns a new task with its own priority and stack size. New tasks are spawned with a normal priority and are in TASK_READY state, waiting for scheduling.

##### ucx_task_cancel()

- Cancels a previously spawned task and removes kernel allocated data structures.

##### ucx_task_yield()

- Yields que processor voluntarily (non-preemptive task reschedule), changing its state TASK_RUNNING to TASK_READY. A task invoking this function gives up execution and calls the scheduler. As a consequence, it is rescheduled to run again in the future.

##### ucx_task_delay()

- Puts the current task in a blocked state changing its state to TASK_BLOCKED for a number of ticks (scheduling events). After the delay, the task state is changed to TASK_READY. If the system is initialized as preemptive, the delay is updated on dispatcher interrupts. Otherwise, *ucx_task_yield()* updates the delay.

##### ucx_task_suspend()

- Puts a task in the TASK_SUSPENDED state until another tasks resumes it from this state.

##### ucx_task_resume()

- Resumes a task from the TASK_SUSPENDED state, returning it to the TASK_READY state.

##### ucx_task_priority()

- Changes a task priority from the default priority. Valid priorities are TASK_IDLE_PRIO, TASK_LOW_PRIO, TASK_BELOW_PRIO, TASK_NORMAL_PRIO (default), TASK_ABOVE_PRIO, TASK_HIGH_PRIO TASK_REALTIME_PRIO and TASK_CRIT_PRIO. These priorities are relative for the task set, according to a priority round-robin scheduler.

##### ucx_task_rt_priority()

- Setup a task realtime priority. The priority is a pointer to a user defined data structure, which holds data that is relevant to a user defined scheduler.

##### ucx_task_id()

- Returns the current task id number.

##### ucx_task_idref()

- Returns the task id of the referenced task pointer.

##### ucx_task_wfi()

- Blocks the current task in a busy wait, until its scheduling quantum expires.

##### ucx_task_count()

- Returns the number of tasks in the system.


#### Coroutine

Coroutines are lightweight, stackless scheduling resources. An application in UCX/OS is typically composed by tasks, tasks with an associated group of coroutines or standalone coroutines. Coroutines communicate using shared memory and are synchronized by exchanging data through pipes (using non-blocking calls only) or event queues.

##### ucx_cr_ginit()

- Creates and initializes a coroutine group context.

##### ucx_cr_gdestroy()

- Destroys a previously initialized coroutine group context.

##### ucx_cr_add()

- Adds a coroutine to a group context, defining its relative priority to other coroutines in the same group.

##### ucx_cr_cancel()

- Removes a coroutine from a group context.

##### ucx_cr_schedule()

- Schedules the coroutine with the highest priority in the same group, according to a priority round-robin scheduling policy.


#### System

##### ucx_ticks()

- Returns the number of executed task dispatches in the system.

##### ucx_uptime()

- Returns the system uptime since boot with microsecond resolution.


#### Semaphore

Semaphore is a basic task synchronization primitive, with Dijkstra's semantics. The implementation of semaphores in the kernel associates a counter and queue for each semaphore instance.

##### ucx_sem_create()

- Creates and initializes a semaphore.

##### ucx_sem_destroy()

- Destroys a semaphore and frees semaphore data structures.

##### ucx_sem_wait()

- Waits on a semaphore. A task can either decrement the semaphore value and pass atomically (semaphore value is > 0 before the call), or block on the semaphore (semaphore value is <= 0 before the call).

##### ucx_sem_trywait()

- Tries to waits on a semaphore. A task can either decrement the semaphore value and pass atomically (semaphore value is > 0 before the call), or return with an error.

##### ucx_sem_signal()

- Signals a semaphore. A task can either increment the semaphore value (semaphore value >= 0 before the call), or increment and unblock a waiting task (semaphore value is < 0 before the call).

#### Pipe

Pipes are basic character oriented communication channels between tasks. Pipes can be used to synchronize and pass data between tasks, and they are implemented using blocking semantics. Each pipe can have a configurable size, essentially acting as a data buffer.

#### Message Queue

Message queues are simple message oriented communication channels between tasks. Message queues can be used to synchronize and pass simple (such as integers, strings) or structured  messages between tasks, and are implemented using non-blocking semantics.

#### Timer

Timers are flexible resources that allow the dispatch of events, implemented as callback functions. Software timers can be used to control a large number of events, without the limitations of hardware timers, such as limited a set of timers and different configurations for each timer. Software timers are handled in a single task and callbacks are dispatched in the context of this task. This reduces resource usage, compared to timers implemented as several tasks and using the *ucx_task_delay()* primitive. Timers can be configured in single shot or auto-reload modes.

Two implementations are provided for timer management. The first one, uses the *timer_handler_systick()* function which uses the system tick as a time reference. The second one uses the *timer_handler()* which is based on the system uptime, based on a running hardware counter as a time reference.


### Device driver API

Device drivers can be implemented with a generic interface or with a custom interface, according to the needs of the specific device or application. The generic interface implements the following operations, and devices are handled as files: *dev_init()*, *dev_deinit()*, *dev_open()*, *dev_close()*, *dev_read()*, *dev_write()*, *dev_seek()* and *dev_ioctl()*. This interface is implemented (or at least, most of this interface) for the majority of devices. This interface can be accessed via direct calls (if a driver implements API function wrappers) or through a pointer in the device descriptor. Other devices may use a different, custom interface, which is specialized according to the specific device.

A typical device driver is saparated in three parts: a) driver interface (API), macros and function wrappers; b) device driver implementation; c) device driver instantiation and function mapping (implementation to interface mapping). An application can create one or more instances of the same driver and it is responsible for the driver setup and management.


#### Generic device driver interface

Drivers that use the generic interface can be accessed via *struct device_api_s* pointer, which is a member of the *struct device_s* device driver structure. An application declares an instance of the device driver structure, structures related to the specific driver (configuration and data) and a pointer to the device driver data structure.

##### dev_init()

- Allocate driver resources, initialize hardware and data structures.

##### dev_deinit()

- Free data structures, deinitialize hardware and free driver resources.

##### dev_open()

- Open the device and lock it for exclusive usage (if applicable).

##### dev_close()

- Close the device and free it, releasing exclusive usage (if applicable).

##### dev_read()

- Perform a read operation, copying data from the device to a buffer.

##### dev_write()

- Perform a write operation, copying buffer data to the device.

##### dev_seek()

- Position the current file pointer to a specific byte on a block oriented device.

##### dev_ioctl()

- Perform a general purpose or device specific IO operation on the device.


#### Custom interfaces

Drivers that use a custom interface can be accessed via *void* pointer, which is a member of the *struct device_s* device driver structure. An application declares an instance of the device driver structure, structures related to the specific driver (configuration and data), a pointer to the device driver data structure and another pointer which specifies the data type of the interface to the custom driver API.


### Library API

Lists and queues are basic data structures which are provided to applications as an API. Lists are implemented as singly or doubly linked lists with sentinel nodes at both ends, so less operations are needed when adding or removing items. Queues are circular data structures and have a defined size on their creation aligned to the next power of two. This results in an efficient implementation of circular queues, as no modular arithmetic needs to be performed for insertion and removal of items.

| List (singly)		| List (doubly)		| Queue			|
| :-------------------- | :-------------------- | :-------------------- |
| list_create()		| dlist_create()	| queue_create()	|
| list_destroy()	| dlist_destroy()	| queue_destroy()	|
| list_push()		| dlist_push()		| queue_count()		|
| list_pushback()	| dlist_pushback()	| queue_enqueue()	|
| list_pop()		| dlist_pop()		| queue_dequeue()	|
| list_popback()	| dlist_popback()	| queue_peek()		|
| list_insert()		| dlist_insert()	|			|
| list_remove()		| dlist_remove()	|			|
| list_index()		| dlist_index()		|			|
| list_foreach()	| dlist_foreach()	|			|


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
| strsep()	| strtok()	| strtok_r()	| strtol()	| atoi()	|
| itoa()	| memcpy()	| memmove()	| memcmp()	| memset()	|
| abs()		| random()	| srand()	| puts()	| gets()	|
| fgets()	| getline()	| printf()	| sprintf()	| free()	|
| malloc()	| calloc()	| realloc()	|
