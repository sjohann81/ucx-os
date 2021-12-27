/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct tcb_s *tcb_p;
static struct tcb_s *tcb_first = 0;
static volatile uint32_t ctx_switches = 0;
static uint16_t id = 0;

static void guard_check(void)
{
	uint32_t check = 0x33333333;
	
	if (*tcb_p->guard_addr != check) {
		hexdump((void *)tcb_p->guard_addr, tcb_p->guard_sz);
		_printf("\n*** HALT - task %d, guard %08x (%d) check failed\n", tcb_p->id,
			(size_t)tcb_p->guard_addr, (size_t)tcb_p->guard_sz);
		for (;;);
	}
		
}

static void update_delay(void)
{
	struct tcb_s *tcb_ptr = tcb_first;
	
	for (;;	tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->state == TASK_BLOCKED && tcb_ptr->delay > 0) {
			tcb_ptr->delay--;
			if (tcb_ptr->delay == 0)
				tcb_ptr->state = TASK_READY;
		}
		if (tcb_ptr->tcb_next == tcb_first) break;
	}
}

void dispatcher(void)
{
	if (!setjmp(tcb_p->context)) {
		update_delay();
		guard_check();
		if (tcb_p->state == TASK_RUNNING)
			tcb_p->state = TASK_READY;
		do {
			tcb_p = tcb_p->tcb_next;
		} while (tcb_p->state != TASK_READY);
		tcb_p->state = TASK_RUNNING;
		ctx_switches++;
		_interrupt_tick();
		longjmp(tcb_p->context, 1);
	}
}

static void sched_init(int32_t preemptive)
{
	tcb_p = tcb_first;
	if (preemptive) {
		_timer_enable();
	}
	(*tcb_p->task)();
}


/* task management */

int32_t ucx_task_add(void *task, uint16_t guard_size)
{
	struct tcb_s *tcb_last = tcb_p;
	
	tcb_p = (struct tcb_s *)_malloc(sizeof(struct tcb_s));
	if (tcb_first == 0)
		tcb_first = tcb_p;

	if (!tcb_p)
		return -1;

	if (tcb_last)
		tcb_last->tcb_next = tcb_p;
	tcb_p->tcb_next = tcb_first;
	tcb_p->task = task;
	tcb_p->delay = 0;
	tcb_p->guard_sz = guard_size;
	tcb_p->id = id++;
	tcb_p->state = TASK_STOPPED;
	
	return 0;
}

/*
 * First following lines of code are absurd at best. Stack marks are
 * used by guard_check() to detect stack overflows (sometimes). It is
 * up to the user to define sufficient stack guard space (considering
 * local thread allocation of the stack for recursion and context
 * saving). We also need the safety pig, just in case.
                         _ 
 _._ _..._ .-',     _.._(`)) 
'-. `     '  /-._.-'    ',/ 
   )         \            '. 
  / _    _    |             \ 
 |  a    a    /              | 
 \   .-.                     ;   
  '-('' ).-'       ,'       ; 
     '-;           |      .' 
        \           \    / 
        | 7  .__  _.-\   \ 
        | |  |  ``/  /`  / 
       /,_|  |   /,_/   / 
          /,_/      '`-' 
*/
void ucx_task_init(void)
{
	char guard[tcb_p->guard_sz];
	
	_memset(guard, 0x69, tcb_p->guard_sz);
	_memset(guard, 0x33, 4);
	_memset((guard) + tcb_p->guard_sz - 4, 0x33, 4);
	tcb_p->guard_addr = (uint32_t *)guard;
	_printf("task %d, guard: %08x - %08x\n", tcb_p->id, (size_t)tcb_p->guard_addr,
		(size_t)tcb_p->guard_addr + tcb_p->guard_sz);
	
	if (!setjmp(tcb_p->context)) {
		tcb_p->state = TASK_READY;
		if (tcb_p->tcb_next == tcb_first) {
			tcb_p->state = TASK_RUNNING;
		} else {
			tcb_p = tcb_p->tcb_next;
			tcb_p->state = TASK_RUNNING;
			(*tcb_p->task)();
		}
	}
	_ei(1);
}

void ucx_task_yield()
{
	if (!setjmp(tcb_p->context)) {
		update_delay();
		guard_check();
		if (tcb_p->state == TASK_RUNNING)
			tcb_p->state = TASK_READY;
		do {
			tcb_p = tcb_p->tcb_next;
		} while (tcb_p->state != TASK_READY);
		tcb_p->state = TASK_RUNNING;
		ctx_switches++;
		longjmp(tcb_p->context, 1);
	}
}

void ucx_task_delay(uint16_t ticks)
{
	tcb_p->delay = ticks;
	tcb_p->state = TASK_BLOCKED;
	ucx_task_yield();
}

int32_t ucx_task_suspend(uint16_t id)
{
	struct tcb_s *tcb_ptr = tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			ucx_enter_critical();
			if (tcb_ptr->state == TASK_READY || tcb_ptr->state == TASK_RUNNING) {
				tcb_ptr->state = TASK_SUSPENDED;
				ucx_leave_critical();
				break;
			} else {
				ucx_leave_critical();
				return -1;
			}
		}
		if (tcb_ptr->tcb_next == tcb_first)
			return -1;
	}
	if (tcb_p->id == id)
		ucx_task_yield();
	
	return 0;
}

int32_t ucx_task_resume(uint16_t id)
{
	struct tcb_s *tcb_ptr = tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			ucx_enter_critical();
			if (tcb_ptr->state == TASK_SUSPENDED) {
				tcb_ptr->state = TASK_READY;
				ucx_leave_critical();
				break;
			} else {
				ucx_leave_critical();
				return -1;
			}
		}	
		if (tcb_ptr->tcb_next == tcb_first)
			return -1;
	}
	if (tcb_p->id == id)
		ucx_task_yield();
	
	return 0;
}


uint16_t ucx_task_id()
{
	return tcb_p->id;
}

void ucx_task_wfi()
{
	volatile uint32_t s;
	
	s = ctx_switches;
	while (s == ctx_switches);
}

uint16_t ucx_tasks()
{
	return id + 1;
}

void ucx_enter_critical()
{
	_timer_disable();
}

void ucx_leave_critical()
{
	_timer_enable();
}


int32_t main(void)
{
	int32_t pr;
	
	_hardware_init();
	_printf("UCX/OS boot on %s\n", __ARCH__);
#ifndef UCX_OS_HEAP_SIZE
	heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	_printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	heap_init((size_t *)&_heap, UCX_OS_HEAP_SIZE);
	_printf("heap_init(), %d bytes free\n", UCX_OS_HEAP_SIZE);
#endif
	pr = app_main();
	sched_init(pr);
	
	return 0;
}
