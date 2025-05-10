/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct kcb_s kernel_state = {
	.tasks = 0,
	.task_current = 0,
	.rt_sched = krnl_noop_rtsched,
	.timer_lst = 0,
	.id_next = 0,
	.ticks = 0
};
	
struct kcb_s *kcb = &kernel_state;


/* kernel auxiliary functions */

static void stack_check(void)
{
	struct tcb_s *task = kcb->task_current->data;
	uint32_t check = 0x33333333;
	uint32_t *stack_p = (uint32_t *)task->stack;

	if (*stack_p != check) {
		printf("\n*** task %d, stack: 0x%p (size %d)\n", task->id,
			task->stack, task->stack_sz);
		krnl_panic(ERR_STACK_CHECK);
	}
		
}

static struct node_s *delay_update(struct node_s *node, void *arg)
{
	struct tcb_s *task = node->data;
	
	if (task->state == TASK_BLOCKED && task->delay > 0) {
		task->delay--;
		if (task->delay == 0)
			task->state = TASK_READY;
	}
	
	return 0;
}

static struct node_s *idcmp(struct node_s *node, void *arg)
{
	struct tcb_s *task = node->data;
	uint16_t id = (size_t)arg;
	
	if (task->id == id)
		return node;
	else
		return 0;
}

static struct node_s *refcmp(struct node_s *node, void *arg)
{
	struct tcb_s *task = node->data;
	
	if (task->task == arg)
		return node;
	else
		return 0;
}

void krnl_panic(uint32_t ecode)
{
	int err;
	
	_di();
	printf("\n*** HALT (%d)", ecode);
	for (err = 0; perror[err].ecode != ERR_UNKNOWN; err++)
		if (perror[err].ecode == ecode) break;
	printf(" - %s\n", perror[err].desc);
	
	for (;;);
}


/* 
 * Kernel scheduler and dispatcher 
 * actual dispatch/yield implementation may be platform dependent
 */
 
void _dispatch(void) __attribute__ ((weak, alias ("dispatch")));
void _yield(void) __attribute__ ((weak, alias ("yield")));

/*
 * The scheduler switches tasks based on task states and priorities, using
 * a priority driven round robin algorithm. Current interrupted task is checked
 * for its state and if RUNNING, it is changed to READY. Task priority is
 * kept in the TCB entry in 16 bits, where the 8 MSBs hold the task
 * priority and the 8 LSBs keep current task priority, decremented on each
 * round - so high priority tasks have a higher chance of 'winning' the cpu.
 * Only a task on the READY state is considered as a viable option. NOTICE - 
 * if no task is ready (e.g. no 'idle' task added to the system and no other
 * task ready) the kernel will hang forever in that inner do .. while loop
 * and this is fine, as there is no hope in such system.
 * 
 * In the end, a task is selected for execution, has its priority reassigned
 * and its state changed to RUNNING.
 */

uint16_t krnl_schedule(void)
{
	int itcnt = 0;
	struct tcb_s *task = kcb->task_current->data;
	struct node_s *node = kcb->task_current;
	
	if (task->state == TASK_RUNNING)
		task->state = TASK_READY;
	
	do {
		do {
			node = list_cnext(kcb->tasks, node);
			task = node->data;

			if (itcnt++ > KRNL_SCHED_IMAX)
				krnl_panic(ERR_NO_TASKS);

		} while (task->state != TASK_READY || task->rt_prio);
	} while (--task->priority & 0xff);
	
	kcb->task_current = node;
	task->priority |= (task->priority >> 8) & 0xff;
	task->state = TASK_RUNNING;
	
	return task->id;
}

int32_t krnl_noop_rtsched(void)
{
	return -1;
}

/*  
 * Kernel task dispatch and yield routines. This is highly platform dependent,
 * so it is implemented by generic calls to _dispatch() and _yield(), defined
 * (or not) in a platform HAL. When not implemented, both dispatch() and yield(),
 * defined as weak symbols will be used by default, and context switch based on
 * setjmp() and longjmp() calls is implemented. On the other hand, if there is
 * specific hardware support or expectations for the context switch, the mechanism
 * should be defined in the HAL, implementing both _dispatch() and _yield().
 * 
 * You are not expected to understand this.
 */

void krnl_dispatcher(void)
{
	kcb->ticks++;
	_dispatch();
}

void dispatch(void)
{
	struct tcb_s *task = kcb->task_current->data;
	
	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);
	
	if (!setjmp(task->context)) {
		stack_check();
		list_foreach(kcb->tasks, delay_update, (void *)0);
		if (kcb->rt_sched() < 0)
			krnl_schedule();
		task = kcb->task_current->data;
		_interrupt_tick();
		longjmp(task->context, 1);
	}
}

void yield(void)
{
	struct tcb_s *task = kcb->task_current->data;
	
	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);
	
	if (!setjmp(task->context)) {
		stack_check();
		if (kcb->preemptive == 'n')
			list_foreach(kcb->tasks, delay_update, (void *)0);
		krnl_schedule();
		task = kcb->task_current->data;
		longjmp(task->context, 1);
	}
}


/* task management API */

int32_t ucx_task_spawn(void *task, uint16_t stack_size)
{
	struct tcb_s *new_tcb;
	struct node_s *new_task;

	new_tcb = malloc(sizeof(struct tcb_s));
		
	if (!new_tcb)
		krnl_panic(ERR_TCB_ALLOC);

	CRITICAL_ENTER();
	
	new_task = list_pushback(kcb->tasks, new_tcb);
	
	if (!new_task)
		krnl_panic(ERR_TCB_ALLOC);
	
	new_task->data = new_tcb;
	new_tcb->task = task;
	new_tcb->rt_prio = 0;
	new_tcb->delay = 0;
	new_tcb->stack_sz = stack_size;
	new_tcb->id = kcb->id_next++;
	new_tcb->state = TASK_STOPPED;
	new_tcb->priority = TASK_NORMAL_PRIO;
	new_tcb->stack = malloc(stack_size);
		
	if (!new_tcb->stack)
		krnl_panic(ERR_STACK_ALLOC);

	CRITICAL_LEAVE();

	memset(new_tcb->stack, 0x69, stack_size);
	memset(new_tcb->stack, 0x33, 4);
	memset((new_tcb->stack) + stack_size - 4, 0x33, 4);
	
	_context_init(&new_tcb->context, (size_t)new_tcb->stack,
		stack_size, (size_t)task);

	printf("task %d: 0x%p, stack: 0x%p, size %d\n", new_tcb->id,
		new_tcb->task, new_tcb->stack, new_tcb->stack_sz);

	new_tcb->state = TASK_READY;

	return ERR_OK;
}

int32_t ucx_task_cancel(uint16_t id)
{
	struct node_s *node;
	struct tcb_s *task;
	
	if (id == ucx_task_id())
		return ERR_TASK_CANT_REMOVE;

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}
	
	task = node->data;
	free(task->stack);
	free(task);
	
	list_remove(kcb->tasks, node);
	CRITICAL_LEAVE();
	
	return ERR_OK;
}

void ucx_task_yield()
{
	_yield();
}

void ucx_task_delay(uint16_t ticks)		// FIXME: delay any task
{
	struct tcb_s *task;
	
	CRITICAL_ENTER();
	task = kcb->task_current->data;
	task->delay = ticks;
	task->state = TASK_BLOCKED;
	CRITICAL_LEAVE();
	ucx_task_yield();
}

int32_t ucx_task_suspend(uint16_t id)
{
	struct node_s *node;
	struct tcb_s *task;

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}

	task = node->data;
	if (task->state == TASK_READY || task->state == TASK_RUNNING) {
		task->state = TASK_SUSPENDED;
	} else {
		CRITICAL_LEAVE();
		
		return ERR_TASK_CANT_SUSPEND;
	}
	CRITICAL_LEAVE();
	
	if (kcb->task_current == node)
		ucx_task_yield();

	return ERR_OK;
}

int32_t ucx_task_resume(uint16_t id)
{
	struct node_s *node;
	struct tcb_s *task;

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}

	task = node->data;
	if (task->state == TASK_SUSPENDED) {
		task->state = TASK_READY;
	} else {
		CRITICAL_LEAVE();
		
		return ERR_TASK_CANT_RESUME;
	}
	CRITICAL_LEAVE();

	return ERR_OK;
}

int32_t ucx_task_priority(uint16_t id, uint16_t priority)
{
	struct node_s *node;
	struct tcb_s *task;

	switch (priority) {
	case TASK_CRIT_PRIO:
	case TASK_REALTIME_PRIO:
	case TASK_HIGH_PRIO:
	case TASK_ABOVE_PRIO:
	case TASK_NORMAL_PRIO:
	case TASK_BELOW_PRIO:
	case TASK_LOW_PRIO:
	case TASK_IDLE_PRIO:
		break;
	default:
		return ERR_TASK_INVALID_PRIO;
	}

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}

	task = node->data;
	task->priority = priority;
	CRITICAL_LEAVE();

	return ERR_OK;
}

int32_t ucx_task_rt_priority(uint16_t id, void *priority)
{
	struct node_s *node;
	struct tcb_s *task;

	if (!priority)
		return ERR_TASK_INVALID_PRIO;

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}

	task = node->data;
	task->rt_prio = priority;
	CRITICAL_LEAVE();

	return ERR_OK;
}

uint16_t ucx_task_id()
{
	struct tcb_s *task = kcb->task_current->data;
	
	return task->id;
}

int32_t ucx_task_idref(void *task)
{
	struct node_s *node;
	struct tcb_s *task_tcb;

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, refcmp, task);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}
	
	task_tcb = node->data;
	CRITICAL_LEAVE();
	
	return task_tcb->id;	
}

void ucx_task_wfi()
{
	volatile uint32_t s;
	
	if (kcb->preemptive == 'n')
		return;
	
	s = kcb->ticks;
	while (s == kcb->ticks);
}

uint16_t ucx_task_count()
{
	return kcb->tasks->length;
}

uint32_t ucx_ticks()
{
	return kcb->ticks;
}

uint64_t ucx_uptime()
{
	return _read_us() / 1000;
}
