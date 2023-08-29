/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct kcb_s kernel_state = {
	.tasks = 0,
	.task_current = 0,
	.events = 0,
	.ctx_switches = 0,
	.id = 0
};
	
struct kcb_s *kcb = &kernel_state;

static struct node_s *idcmp(struct node_s *node, void *arg)
{
	struct tcb_s *task = node->data;
	uint16_t id = (size_t)arg;
	
	if (task->id == id)
		return node;
	else
		return 0;
}


/* task management / API */

int32_t ucx_task_add(void *task, uint16_t stack_size)
{
	struct tcb_s *new_tcb = malloc(sizeof(struct tcb_s)); // FIXME: not thread safe
	struct node_s *new_task;

	CRITICAL_ENTER();
		
	if (!new_tcb)
		ucx_panic(ERR_TCB_ALLOC);
	
	new_task = list_pushback(kcb->tasks, new_tcb);
	
	if (!new_task)
		ucx_panic(ERR_TCB_ALLOC);
	
	new_task->data = new_tcb;
	new_tcb->task = task;
	new_tcb->delay = 0;
	new_tcb->stack_sz = stack_size;
	new_tcb->id = kcb->id++;
	new_tcb->state = TASK_STOPPED;
	new_tcb->priority = TASK_NORMAL_PRIO;
	new_tcb->stack = malloc(stack_size);
		
	if (!new_tcb->stack)
		ucx_panic(ERR_STACK_ALLOC);

	CRITICAL_LEAVE();

	memset(new_tcb->stack, 0x69, stack_size);
	memset(new_tcb->stack, 0x33, 4);
	memset((new_tcb->stack) + stack_size - 4, 0x33, 4);
	
	_context_init(&new_tcb->context, (size_t)new_tcb->stack,
		stack_size, (size_t)task);

	printf("task %d: %08x, stack: %08x, size %d\n", new_tcb->id,
		(uint32_t)new_tcb->task, (uint32_t)new_tcb->stack,
		new_tcb->stack_sz);
	
	new_tcb->state = TASK_READY;

	return ERR_OK;
}

int32_t ucx_task_remove(uint16_t id)
{
	return 0;
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
/*	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
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
*/	
	return 0;
}

int32_t ucx_task_resume(uint16_t id)
{
/*	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
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
*/	
	return 0;
}

int32_t ucx_task_priority(uint16_t id, uint16_t priority)
{
	struct node_s *node;
	struct tcb_s *task;

	switch (priority) {
	case TASK_CRIT_PRIO:
	case TASK_HIGH_PRIO:
	case TASK_NORMAL_PRIO:
	case TASK_LOW_PRIO:
	case TASK_IDLE_PRIO:
		break;
	default:
		return ERR_TASK_INVALID_PRIO;
	}

	CRITICAL_ENTER();
	node = list_foreach(kcb->tasks, idcmp, (void *)id);
	
	if (!node) {
		CRITICAL_LEAVE();
		
		return ERR_TASK_NOT_FOUND;
	}

	task = node->data;
	task->priority = priority;
	CRITICAL_LEAVE();

	return ERR_OK;
}

uint16_t ucx_task_id()
{
	struct tcb_s *task = kcb->task_current->data;
	
	return task->id;
}

void ucx_task_wfi()
{
	volatile uint32_t s;
	
	s = kcb->ctx_switches;
	while (s == kcb->ctx_switches);
}

uint16_t ucx_task_count()
{
	return kcb->tasks->length;
}

void ucx_panic(uint32_t ecode)
{
	int err;
	
	_di();
	printf("\n*** HALT (%08x) - ", ecode);
	for (err = 0; perror[err].ecode != ERR_UNKNOWN; err++)
		if (perror[err].ecode == ecode) break;
	printf("%s\n", perror[err].desc);
	
	for (;;);
}
