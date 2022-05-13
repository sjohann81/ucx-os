#include <hal.h>
#include <libc.h>
#include <dump.h>
#include <list.h>
#include <queue.h>
#include <pipe.h>
#include <semaphore.h>
#include <malloc.h>
#include <stdarg.h>

/* task priorities */
#define TASK_CRIT_PRIO		((0x03 << 8) | 0x03)		/* priority 0 .. 3 */
#define TASK_HIGH_PRIO 		((0x0f << 8) | 0x0f)		/* priority 4 .. 15 */
#define TASK_NORMAL_PRIO	((0x1f << 8) | 0x1f)		/* priority 16 .. 31 */
#define TASK_LOW_PRIO			((0x3f << 8) | 0x3f)		/* priority 32 .. 63 */
#define TASK_IDLE_PRIO		((0x7f << 8) | 0x7f)		/* priority 64 .. 127 */

/* task states */
enum {TASK_STOPPED, TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_SUSPENDED};

struct rt_par_s {
	uint16_t period;
	uint16_t capacity;
	uint16_t deadline;
	uint16_t rem_period;
	uint16_t rem_capacity;
	uint16_t rem_deadline;
};

/* task control block node */
struct tcb_s {
	struct tcb_s *tcb_next;
	struct rt_par_s *p_params;
	void (*task)(void);
	jmp_buf context;
	uint32_t *guard_addr;
	uint16_t guard_sz;
	uint16_t id;
	uint16_t delay;
	uint16_t priority;
	uint8_t state;
};

/* kernel control block */
struct kcb_s {
	struct tcb_s *tcb_p;
	struct tcb_s *tcb_first;
	volatile uint32_t ctx_switches;
	volatile uint32_t dln_miss;
	volatile uint32_t job_exec;
	volatile uint32_t prd_exec;
	uint16_t id;
};

/* kernel base API */
int32_t ucx_task_add(void *task, uint16_t guard_size);
int32_t ucx_task_add_periodic(void *task, uint16_t period, uint16_t capacity, uint16_t deadline, uint16_t guard_size);
void ucx_task_init();
void ucx_task_yield();
void ucx_task_delay(uint16_t ticks);
int32_t ucx_task_suspend(uint16_t id);
int32_t ucx_task_resume(uint16_t id);
int32_t ucx_task_priority(uint16_t id, uint16_t priority);
uint16_t ucx_task_id();
void ucx_task_wfi();
uint16_t ucx_task_count();
uint32_t ucx_context_switch();
uint32_t ucx_deadline_misses();
uint32_t ucx_job_executed();
uint32_t ucx_period_executed();
void ucx_critical_enter();
void ucx_critical_leave();
int32_t app_main();
