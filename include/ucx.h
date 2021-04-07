#include <hal.h>
#include <libc.h>
#include <list.h>
#include <queue.h>
#include <pipe.h>
#include <malloc.h>
#include <stdarg.h>

enum {TASK_STOPPED, TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_INTERRUPTED};

int32_t ucx_task_add(void *task);
void ucx_task_init(char *guard, uint16_t guard_size);
void ucx_task_yield();
uint16_t ucx_task_id();
void ucx_task_wfi();
void ucx_enter_critical();
void ucx_leave_critical();
int32_t app_main();
