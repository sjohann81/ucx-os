#include <hal.h>
#include <libc.h>
#include <list.h>
#include <queue.h>
#include <pipe.h>
#include <semaphore.h>
#include <malloc.h>
#include <stdarg.h>

enum {TASK_STOPPED, TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_SUSPENDED};

struct tcb_s {
	struct tcb_s *tcb_next;
	void (*task)(void);
	jmp_buf context;
	uint32_t *guard_addr;
	uint16_t guard_sz;
	uint16_t id;
	uint16_t delay;
	uint8_t state;
};

int32_t ucx_task_add(void *task, uint16_t guard_size);
void ucx_task_init();
void ucx_task_yield();
void ucx_task_delay(uint16_t ticks);
uint16_t ucx_task_id();
void ucx_task_wfi();
uint16_t ucx_tasks();
void ucx_enter_critical();
void ucx_leave_critical();
int32_t app_main();
