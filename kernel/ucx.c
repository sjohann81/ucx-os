/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct kcb_s kernel_state = {
	.tcb_p = 0,
	.tcb_first = 0,
	.events = 0,
	.ctx_switches = 0,
	.id = 0
};
	
struct kcb_s *kcb_p = &kernel_state;


/* task management / API */

int32_t ucx_task_add(void *task, uint16_t stack_size)
{
	struct tcb_s *tcb_last = kcb_p->tcb_p;
	
	kcb_p->tcb_p = (struct tcb_s *)malloc(sizeof(struct tcb_s));
	
	if (kcb_p->tcb_first == 0)
		kcb_p->tcb_first = kcb_p->tcb_p;

	if (!kcb_p->tcb_p)
		return -1;
		
	CRITICAL_ENTER();
	if (tcb_last)
		tcb_last->tcb_next = kcb_p->tcb_p;

	kcb_p->tcb_p->tcb_next = kcb_p->tcb_first;
	kcb_p->tcb_p->task = task;
	kcb_p->tcb_p->delay = 0;
	kcb_p->tcb_p->stack_sz = stack_size;
	kcb_p->tcb_p->id = kcb_p->id++;
	kcb_p->tcb_p->state = TASK_STOPPED;
	kcb_p->tcb_p->priority = TASK_NORMAL_PRIO;
	kcb_p->tcb_p->stack = malloc(kcb_p->tcb_p->stack_sz);
	
	if (!kcb_p->tcb_p->stack) {
		printf("\n*** HALT - task %d, stack alloc failed\n", kcb_p->tcb_p->id);
		
		for (;;);
	}
	
	memset(kcb_p->tcb_p->stack, 0x69, kcb_p->tcb_p->stack_sz);
	memset(kcb_p->tcb_p->stack, 0x33, 4);
	memset((kcb_p->tcb_p->stack) + kcb_p->tcb_p->stack_sz - 4, 0x33, 4);
	
	_context_init(&kcb_p->tcb_p->context, (size_t)kcb_p->tcb_p->stack,
		kcb_p->tcb_p->stack_sz, (size_t)task);
	CRITICAL_LEAVE();
	
	printf("task %d: %08x, stack: %08x, size %d\n", kcb_p->tcb_p->id,
		(uint32_t)kcb_p->tcb_p->task, (uint32_t)kcb_p->tcb_p->stack,
		kcb_p->tcb_p->stack_sz);
	
	kcb_p->tcb_p->state = TASK_READY;

	/* FIXME: return task id */
	return 0;
}

void ucx_task_delay(uint16_t ticks)
{
	CRITICAL_ENTER();
	kcb_p->tcb_p->delay = ticks;
	kcb_p->tcb_p->state = TASK_BLOCKED;
	CRITICAL_LEAVE();
	ucx_task_yield();
}

int32_t ucx_task_suspend(uint16_t id)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			CRITICAL_ENTER();
			if (tcb_ptr->state == TASK_READY || tcb_ptr->state == TASK_RUNNING) {
				tcb_ptr->state = TASK_SUSPENDED;
				CRITICAL_LEAVE();
				break;
			} else {
				CRITICAL_LEAVE();
				return -1;
			}
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first)
			return -1;
	}
	
	if (kcb_p->tcb_p->id == id)
		ucx_task_yield();
	
	return 0;
}

int32_t ucx_task_resume(uint16_t id)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			CRITICAL_ENTER();
			if (tcb_ptr->state == TASK_SUSPENDED) {
				tcb_ptr->state = TASK_READY;
				CRITICAL_LEAVE();
				break;
			} else {
				CRITICAL_LEAVE();
				return -1;
			}
		}	
		if (tcb_ptr->tcb_next == kcb_p->tcb_first)
			return -1;
	}
	if (kcb_p->tcb_p->id == id)
		ucx_task_yield();
	
	return 0;
}

int32_t ucx_task_priority(uint16_t id, uint16_t priority)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;

	switch (priority) {
	case TASK_CRIT_PRIO:
	case TASK_HIGH_PRIO:
	case TASK_NORMAL_PRIO:
	case TASK_LOW_PRIO:
	case TASK_IDLE_PRIO:
		break;
	default:
		return -1;
	}
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			CRITICAL_ENTER();
			tcb_ptr->priority = priority;
			CRITICAL_LEAVE();
			break;
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first)
			return -1;
	}
	
	return 0;
}

uint16_t ucx_task_id()
{
	return kcb_p->tcb_p->id;
}

void ucx_task_wfi()
{
	volatile uint32_t s;
	
	s = kcb_p->ctx_switches;
	while (s == kcb_p->ctx_switches);
}

uint16_t ucx_task_count()
{
	return kcb_p->id + 1;
}
