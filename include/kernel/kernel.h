/* task priorities */
enum task_priorities {
	TASK_CRIT_PRIO		= ((0x01 << 8) | 0x01),		/* priority 1 */
	TASK_REALTIME_PRIO	= ((0x03 << 8) | 0x03),		/* priority 2 .. 3 */
	TASK_HIGH_PRIO		= ((0x07 << 8) | 0x07),		/* priority 4 .. 7 */
	TASK_ABOVE_PRIO		= ((0x0f << 8) | 0x0f),		/* priority 8 .. 15 */
	TASK_NORMAL_PRIO	= ((0x1f << 8) | 0x1f),		/* priority 16 .. 31 */
	TASK_BELOW_PRIO		= ((0x3f << 8) | 0x3f),		/* priority 32 .. 63 */
	TASK_LOW_PRIO		= ((0x7f << 8) | 0x7f),		/* priority 64 .. 127 */
	TASK_IDLE_PRIO		= ((0xff << 8) | 0xff)		/* priority 128 .. 255 */
};

/* task states */
enum task_states {TASK_STOPPED, TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_SUSPENDED};

/* task control block node */
struct tcb_s {
	void (*task)(void);
	jmp_buf context;		/* jmp_buf is architecture specific */
	size_t *stack;
	size_t stack_sz;
	void *rt_prio;
	uint16_t id;
	uint16_t delay;
	uint16_t priority;
	uint8_t state;
};

/* kernel control block */
struct kcb_s {
	struct list_s *tasks;
	struct node_s *task_current;
	jmp_buf context;
	int32_t (*rt_sched)(void);
	struct list_s *timer_lst;
	volatile uint32_t ticks;
	uint16_t id_next;
	char preemptive;
};

extern struct kcb_s *kcb;

#define KRNL_SCHED_IMAX		10000

/* kernel API */
#define CRITICAL_ENTER()({kcb->preemptive == 'y' ? _di() : 0; })	// shouln't be always _di()?
#define CRITICAL_LEAVE()({kcb->preemptive == 'y' ? _ei() : 0; })	// shouln't be always _ei()?
#define NOSCHED_ENTER()({kcb->preemptive == 'y' ? _timer_disable() : 0; })
#define NOSCHED_LEAVE()({kcb->preemptive == 'y' ? _timer_enable() : 0; })

void krnl_panic(uint32_t ecode);
uint16_t krnl_schedule(void);
int32_t krnl_noop_rtsched(void);
void krnl_dispatcher(void);
/* actual dispatch/yield implementation may be platform dependent */
void _dispatch(void);
void _yield(void);

/* task management API */
int32_t ucx_task_spawn(void *task, uint16_t stack_size);
int32_t ucx_task_cancel(uint16_t id);
void ucx_task_yield();
void ucx_task_delay(uint16_t ticks);
int32_t ucx_task_suspend(uint16_t id);
int32_t ucx_task_resume(uint16_t id);
int32_t ucx_task_priority(uint16_t id, uint16_t priority);
int32_t ucx_task_rt_priority(uint16_t id, void *priority);
uint16_t ucx_task_id();
int32_t ucx_task_idref(void *task);
void ucx_task_wfi();
uint16_t ucx_task_count();
uint32_t ucx_ticks();
uint64_t ucx_uptime();

int32_t app_main();
