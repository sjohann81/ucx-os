/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 *                11/2025 (multicore)
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

#ifndef MULTICORE

struct kcb_s kernel_state = {
	.tasks = 0,
	.task_current = 0,
	.rt_sched = krnl_noop_rtsched,
	.timer_lst = 0,
	.ticks = 0,
	.id_next = 0,
	.preemptive = 'n'
};
	
struct kcb_s *kcb = &kernel_state;

#else

struct kcb_s kernel_state[MAX_CORES] = {
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'},
	{.tasks = 0, .task_current = 0, .rt_sched = krnl_noop_rtsched, .timer_lst = 0, .ticks = 0, .id_next = 0, .preemptive = 'n'}
};
	
struct kcb_s *kcb[MAX_CORES] = {
	&kernel_state[0], &kernel_state[1],
	&kernel_state[2], &kernel_state[3],
	&kernel_state[4], &kernel_state[5],
	&kernel_state[6], &kernel_state[7]
};

struct spinlock_s global_lock = { .locked = 0, .cpu = -1 };

#endif


/* kernel auxiliary functions */

static void stack_check(void)
{
#ifndef MULTICORE
	struct tcb_s *task = kcb->task_current->data;
#else
	struct tcb_s *task = kcb[_cpu_id()]->task_current->data;
#endif
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

#ifdef CONFIG_SCHED_SIMPLE
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

#ifndef MULTICORE
	struct tcb_s *task = kcb->task_current->data;
	struct node_s *node = kcb->task_current;
#else
	struct tcb_s *task = kcb[_cpu_id()]->task_current->data;
	struct node_s *node = kcb[_cpu_id()]->task_current;
#endif	
	
	if (task->state == TASK_RUNNING)
		task->state = TASK_READY;
	
	do {
		do {
#ifndef MULTICORE
			node = list_cnext(kcb->tasks, node);
#else
			node = list_cnext(kcb[_cpu_id()]->tasks, node);
#endif
			task = node->data;

			if (itcnt++ > 10000)
				krnl_panic(ERR_NO_TASKS);

		} while (task->state != TASK_READY || task->rt_prio);
	} while (--task->priority & 0xff);

#ifndef MULTICORE	
	kcb->task_current = node;
#else
	kcb[_cpu_id()]->task_current = node;
#endif
	task->priority |= (task->priority >> 8) & 0xff;
	task->state = TASK_RUNNING;
	
	return task->id;
}

#else

/*
 * The scheduler switches tasks based on task states and priorities, using
 * a priority driven round robin algorithm. Current interrupted task is
 * checked for its state and if RUNNING, it is changed to READY. Task
 * priority is kept in the TCB entry in 16 bits, where the 8 MSBs hold
 * the task priority and the 8 LSBs keep current task priority. This
 * algorithm traverses the task list twice. In the first run, the highest
 * priority task is selected. On the second run, the priority from the
 * selected task is subtracted from all other ready tasks. Only a task
 * on the READY state is considered to be scheduled.
 * 
 * In the end, a task is selected for execution, has its priority reassigned
 * and its state changed to RUNNING.
 */

uint16_t krnl_schedule(void)
{
#ifndef MULTICORE
	struct tcb_s *task = kcb->task_current->data;
#else
	struct tcb_s *task = kcb[_cpu_id()]->task_current->data;
#endif
	struct node_s *node, *select;
	struct tcb_s *tselect;
	uint16_t priority;
	
	if (task->state == TASK_RUNNING)
		task->state = TASK_READY;

#ifndef MULTICORE
	select = kcb->tasks->head->next;
	node = kcb->tasks->head;
#else
	select = kcb[_cpu_id()]->tasks->head->next;
	node = kcb[_cpu_id()]->tasks->head;
#endif
	tselect = select->data;

	while ((node = list_next(node))) {
		if (!node->next) break;
		task = node->data;
		if ((task->priority & 0xff) <= (tselect->priority & 0xff)) {
			if (task->state == TASK_READY && !task->rt_prio) {
				select = node;
				tselect = select->data;
			}
		}
	};
	
	tselect = select->data;
	if (tselect->state != TASK_READY || tselect->rt_prio)
		krnl_panic(ERR_NO_TASKS);
	
	priority = tselect->priority;
#ifndef MULTICORE
	node = kcb->tasks->head;
#else
	node = kcb[_cpu_id()]->tasks->head;
#endif
	while ((node = list_next(node))) {
		if (!node->next) break;
		task = node->data;
		if (task->state == TASK_READY && !task->rt_prio)
			task->priority -= (priority & 0xff);
	};

#ifndef MULTICORE
	kcb->task_current = select;
#else
	kcb[_cpu_id()]->task_current = select;
#endif
	tselect->priority |= (tselect->priority >> 8) & 0xff;
	tselect->state = TASK_RUNNING;

	return tselect->id;
}
#endif

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
#ifndef MULTICORE
	kcb->ticks++;
#else
	kcb[_cpu_id()]->ticks++;
#endif
	_dispatch();
}

void dispatch(void)
{
#ifndef MULTICORE
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
#else
	struct tcb_s *task = kcb[_cpu_id()]->task_current->data;
	
	if (!kcb[_cpu_id()]->tasks->length)
		krnl_panic(ERR_NO_TASKS);
	
	if (!setjmp(task->context)) {
		stack_check();
		list_foreach(kcb[_cpu_id()]->tasks, delay_update, (void *)0);
		if (kcb[_cpu_id()]->rt_sched() < 0)
			krnl_schedule();
		task = kcb[_cpu_id()]->task_current->data;
		_interrupt_tick();
		longjmp(task->context, 1);
	}
#endif
}

void yield(void)
{
	volatile uint32_t s;
	struct tcb_s *task;
	
#ifndef MULTICORE
	if (kcb->preemptive == 'y') {
		s = kcb->ticks;
		_cpu_idle();
		while (s == kcb->ticks);
		
		return;
	}

	CRITICAL_ENTER();
	task = kcb->task_current->data;
	
	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);
	
	if (!setjmp(task->context)) {
		stack_check();
		list_foreach(kcb->tasks, delay_update, (void *)0);
		krnl_schedule();
		task = kcb->task_current->data;
		CRITICAL_LEAVE();
		longjmp(task->context, 1);
	}
	CRITICAL_LEAVE();
#else
	if (kcb[_cpu_id()]->preemptive == 'y') {
		s = kcb[_cpu_id()]->ticks;
		_cpu_idle();
		while (s == kcb[_cpu_id()]->ticks);
		
		return;
	}
	
	CRITICAL_ENTER();
	task = kcb[_cpu_id()]->task_current->data;
	
	if (!kcb[_cpu_id()]->tasks->length)
		krnl_panic(ERR_NO_TASKS);
	
	if (!setjmp(task->context)) {
		stack_check();
		list_foreach(kcb[_cpu_id()]->tasks, delay_update, (void *)0);
		krnl_schedule();
		task = kcb[_cpu_id()]->task_current->data;
		CRITICAL_LEAVE();
		longjmp(task->context, 1);
	}
	CRITICAL_LEAVE();
#endif
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

#ifndef MULTICORE
	new_task = list_pushback(kcb->tasks, new_tcb);
#else
	new_task = list_pushback(kcb[_cpu_id()]->tasks, new_tcb);
#endif
	
	if (!new_task)
		krnl_panic(ERR_TCB_ALLOC);
	
	new_task->data = new_tcb;
	new_tcb->task = task;
	new_tcb->rt_prio = 0;
	new_tcb->delay = 0;
	new_tcb->stack_sz = stack_size;
#ifndef MULTICORE
	new_tcb->id = kcb->id_next++;
#else
	new_tcb->id = kcb[_cpu_id()]->id_next++;
#endif
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

	printf("core %d, task %d: 0x%p, stack: 0x%p, size %d\n", _cpu_id(),
		new_tcb->id, new_tcb->task, new_tcb->stack, new_tcb->stack_sz);

	new_tcb->state = TASK_READY;

	return new_tcb->id;
}

int32_t ucx_task_cancel(uint16_t id)
{
	struct node_s *node;
	struct tcb_s *task;
	
	if (id == ucx_task_id())
		return ERR_TASK_CANT_REMOVE;

	CRITICAL_ENTER();
#ifndef MULTICORE
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
#else
	node = list_foreach(kcb[_cpu_id()]->tasks, idcmp, (void *)(size_t)id);
#endif
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}
	
	task = node->data;
	free(task->stack);
	free(task);

#ifndef MULTICORE
	list_remove(kcb->tasks, node);
#else
	list_remove(kcb[_cpu_id()]->tasks, node);
#endif
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
#ifndef MULTICORE
	task = kcb->task_current->data;
#else
	task = kcb[_cpu_id()]->task_current->data;
#endif
	task->delay = ticks;
	task->state = TASK_BLOCKED;
	CRITICAL_LEAVE();
	_yield();
}

int32_t ucx_task_suspend(uint16_t id)
{
	struct node_s *node;
	struct tcb_s *task;

	CRITICAL_ENTER();
#ifndef MULTICORE
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
#else
	node = list_foreach(kcb[_cpu_id()]->tasks, idcmp, (void *)(size_t)id);
#endif
	
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

#ifndef MULTICORE
	if (kcb->task_current == node)
#else
	if (kcb[_cpu_id()]->task_current == node)
#endif
		_yield();

	return ERR_OK;
}

int32_t ucx_task_resume(uint16_t id)
{
	struct node_s *node;
	struct tcb_s *task;

	CRITICAL_ENTER();
#ifndef MULTICORE
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
#else
	node = list_foreach(kcb[_cpu_id()]->tasks, idcmp, (void *)(size_t)id);
#endif
	
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
#ifndef MULTICORE
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
#else
	node = list_foreach(kcb[_cpu_id()]->tasks, idcmp, (void *)(size_t)id);
#endif
	
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
#ifndef MULTICORE
	node = list_foreach(kcb->tasks, idcmp, (void *)(size_t)id);
#else
	node = list_foreach(kcb[_cpu_id()]->tasks, idcmp, (void *)(size_t)id);
#endif
	
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
#ifndef MULTICORE
	struct tcb_s *task = kcb->task_current->data;
#else
	struct tcb_s *task = kcb[_cpu_id()]->task_current->data;
#endif
	
	return task->id;
}

int32_t ucx_task_idref(void *task)
{
	struct node_s *node;
	struct tcb_s *task_tcb;

	CRITICAL_ENTER();
#ifndef MULTICORE
	node = list_foreach(kcb->tasks, refcmp, task);
#else
	node = list_foreach(kcb[_cpu_id()]->tasks, refcmp, task);
#endif
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}
	
	task_tcb = node->data;
	CRITICAL_LEAVE();
	
	return task_tcb->id;	
}

uint16_t ucx_task_count()
{
#ifndef MULTICORE
	return kcb->tasks->length;
#else
	return kcb[_cpu_id()]->tasks->length;
#endif
}

uint32_t ucx_ticks()
{
#ifndef MULTICORE
	return kcb->ticks;
#else
	return kcb[_cpu_id()]->ticks;
#endif
}

uint64_t ucx_uptime()
{
	return _read_us() / 1000;
}
