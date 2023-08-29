/* file:          sched.c
 * description:   UCX/OS kernel scheduler
 * date:          05/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>


/* actual dispatch/yield implementation may be platform dependent */
void _dispatch(void) __attribute__ ((weak, alias ("dispatch")));
void _yield(void) __attribute__ ((weak, alias ("yield")));


/* kernel auxiliary functions */

static void stack_check(void)
{
	struct tcb_s *task = kcb->task_current->data;
	uint32_t check = 0x33333333;
	uint32_t *stack_p = (uint32_t *)task->stack;

	if (*stack_p != check) {
		hexdump((void *)task->stack, task->stack_sz);
		printf("\n*** task %d, stack: %08x (size %d)\n", task->id,
			(uint32_t)task->stack, (uint32_t)task->stack_sz);
		ucx_panic(ERR_STACK_CHECK);
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
	struct tcb_s *task = kcb->task_current->data;
	
	if (task->state == TASK_RUNNING)
		task->state = TASK_READY;
		
	do {
		do {
			kcb->task_current = kcb->task_current->next;
			if (kcb->task_current == kcb->tasks->tail)
				kcb->task_current = kcb->tasks->head->next;
				
			task = kcb->task_current->data;
		} while (task->state != TASK_READY);
	} while (--task->priority & 0xff);
	
	task->priority |= (task->priority >> 8) & 0xff;
	task->state = TASK_RUNNING;
	kcb->ctx_switches++;
	
	return task->id;
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
	_dispatch();
}

void dispatch(void)
{
	struct tcb_s *task = kcb->task_current->data;
	
	if (!setjmp(task->context)) {
		stack_check();
		list_foreach(kcb->tasks, delay_update, (void *)0);
		krnl_schedule();
		_interrupt_tick();
		task = kcb->task_current->data;
		longjmp(task->context, 1);
	}
}

void yield(void)
{
	struct tcb_s *task = kcb->task_current->data;
	
	if (!setjmp(task->context)) {
		stack_check();
		if (kcb->preemptive == 'n')
			list_foreach(kcb->tasks, delay_update, (void *)0);
		krnl_schedule();
		task = kcb->task_current->data;
		longjmp(task->context, 1);
	}
}
