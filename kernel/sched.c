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

static void krnl_stack_check(void)
{
	uint32_t check = 0x33333333;
	uint32_t *stack_p = (uint32_t *)kcb_p->tcb_p->stack;
	
	if (*stack_p != check) {
		ucx_hexdump((void *)kcb_p->tcb_p->stack, kcb_p->tcb_p->stack_sz);
		printf("\n*** HALT - task %d, stack %08x (%d) check failed\n", kcb_p->tcb_p->id,
			(uint32_t)kcb_p->tcb_p->stack, (uint32_t)kcb_p->tcb_p->stack_sz);
		for (;;);
	}
		
}

static void krnl_delay_update(void)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;;	tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->state == TASK_BLOCKED && tcb_ptr->delay > 0) {
			tcb_ptr->delay--;
			if (tcb_ptr->delay == 0)
				tcb_ptr->state = TASK_READY;
		}
		if (tcb_ptr->tcb_next == kcb_p->tcb_first) break;
	}
}


/* task scheduler and dispatcher */

uint16_t krnl_schedule(void)
{
	if (kcb_p->tcb_p->state == TASK_RUNNING)
		kcb_p->tcb_p->state = TASK_READY;
	
	do {
		do {
			kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
		} while (kcb_p->tcb_p->state != TASK_READY);
	} while (--kcb_p->tcb_p->priority & 0xff);
	
	kcb_p->tcb_p->priority |= (kcb_p->tcb_p->priority >> 8) & 0xff;
	kcb_p->tcb_p->state = TASK_RUNNING;
	kcb_p->ctx_switches++;
	
	return kcb_p->tcb_p->id;
}

void krnl_dispatcher(void)
{
	_dispatch();
}

void ucx_task_yield()
{
	_yield();
}

void dispatch(void)
{
	if (!setjmp(kcb_p->tcb_p->context)) {
		krnl_delay_update();
		krnl_stack_check();
		krnl_schedule();
		_interrupt_tick();
		longjmp(kcb_p->tcb_p->context, 1);
	}
}

void yield(void)
{
	if (!setjmp(kcb_p->tcb_p->context)) {
		/* TODO: check if we need to run a delay update on yields. maybe only on a non-preemtive execution? */ 
		krnl_delay_update();
		krnl_stack_check();
		krnl_schedule();
		longjmp(kcb_p->tcb_p->context, 1);
	}
}
