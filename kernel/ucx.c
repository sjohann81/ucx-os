/* file:          ucx.c
 * description:   UCX/OS kernel
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct kcb_s kernel_state;
struct kcb_s *kcb_p = &kernel_state;

/* kernel auxiliary functions */

static void krnl_guard_check(void)
{
	uint32_t check = 0x33333333;
	
	if (*kcb_p->tcb_p->guard_addr != check) {
		ucx_hexdump((void *)kcb_p->tcb_p->guard_addr, kcb_p->tcb_p->guard_sz);
		printf("\n*** HALT - task %d, guard %08x (%d) check failed\n", kcb_p->tcb_p->id,
			(size_t)kcb_p->tcb_p->guard_addr, (size_t)kcb_p->tcb_p->guard_sz);
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

static void krnl_sched_init(int32_t preemptive)
{
	kcb_p->tcb_p = kcb_p->tcb_first;
	if (preemptive) {
		_timer_enable();
	}
	(*kcb_p->tcb_p->task)();
}


/* task scheduler and dispatcher */

uint16_t krnl_rt_schedule(void)
{
	struct tcb_s *tcb = kcb_p->tcb_p;
	uint16_t deadline = 0xFFFF;
	uint16_t id = 0xFFFF;
	
	/* move a tarefa executando para estado ready */
	if (kcb_p->tcb_p->state == TASK_RUNNING) {
		/* decrementa capacidade se a tarefa periodica está executando */
		if (kcb_p->tcb_p->p_params) {
			/* condição para ignorar primeira execução */
			if (kcb_p->ctx_switches != 0) kcb_p->tcb_p->p_params->rem_capacity--;
		}
		kcb_p->tcb_p->state = TASK_READY;
	}

	do {
		if (kcb_p->tcb_p->p_params) {
			/* decrementa deadline */
			if (kcb_p->tcb_p->p_params->rem_deadline > 0) {
				/* condição para ignorar primeira execução */
				if (kcb_p->ctx_switches != 0) kcb_p->tcb_p->p_params->rem_deadline--;
			} else {
				/* se o deadline for zero e ainda há capacidade (deadline_miss) */
				if (kcb_p->tcb_p->p_params->rem_capacity > 0) kcb_p->dln_miss++;
			}

			/* decrementa periodo */
			if (kcb_p->tcb_p->p_params->rem_period > 0) {
				/* condição para ignorar primeira execução */
				if (kcb_p->ctx_switches != 0) kcb_p->tcb_p->p_params->rem_period--;
			} else {
				/* restaura capacidade, deadline e periodo */
				kcb_p->tcb_p->p_params->rem_capacity = kcb_p->tcb_p->p_params->capacity;
				kcb_p->tcb_p->p_params->rem_deadline = kcb_p->tcb_p->p_params->deadline;
				kcb_p->tcb_p->p_params->rem_period = kcb_p->tcb_p->p_params->period;
				kcb_p->prd_exec++;
			}

			/* verifica se a tarefa periodica ainda tem capacidade */
			if (kcb_p->tcb_p->p_params->rem_capacity > 0) {
				/* testa o deadline para eleger a tarefa com menor deadline (kcb_p->tcb_p) */
				if (deadline > kcb_p->tcb_p->p_params->rem_deadline) {
					deadline = kcb_p->tcb_p->p_params->rem_deadline;
					id = kcb_p->tcb_p->id;
				}
			}
		}
	
		/* aponta para a proxima tarefa da fila */
		kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
	} while (kcb_p->tcb_p != tcb);

	/* verifica se foi eleita alguma tarefa periodica */
	if (id != 0xFFFF) {
		/* move o ponteiro do kernel para tarefa periodica que deve executar */
		while (kcb_p->tcb_p->id != id) kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;

		kcb_p->tcb_p->state = TASK_RUNNING;
		kcb_p->ctx_switches++;
		kcb_p->job_exec++;
		printf("tp %d %d %d\n", kcb_p->tcb_p->id, kcb_p->tcb_p->p_params->rem_capacity, kcb_p->tcb_p->p_params->rem_deadline);	
		return kcb_p->tcb_p->id;
	}

	return 0xFFFF;
}

uint16_t krnl_schedule(void)
{
	if (kcb_p->tcb_p->state == TASK_RUNNING)
		kcb_p->tcb_p->state = TASK_READY;
	do {
		do {
			kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
		} while ((kcb_p->tcb_p->state != TASK_READY) || (kcb_p->tcb_p->p_params));
	} while (--kcb_p->tcb_p->priority & 0xff);

	kcb_p->tcb_p->priority |= (kcb_p->tcb_p->priority >> 8) & 0xff;
	kcb_p->tcb_p->state = TASK_RUNNING;
	kcb_p->ctx_switches++;
	
	printf("ta %d\n", kcb_p->tcb_p->id);
	return kcb_p->tcb_p->id;
}

void krnl_dispatcher(void)
{
	if (!setjmp(kcb_p->tcb_p->context)) {
		krnl_delay_update();
		krnl_guard_check();
		if (krnl_rt_schedule() == 0xFFFF) krnl_schedule();
		_interrupt_tick();
		longjmp(kcb_p->tcb_p->context, 1);
	}
}


/* task management API */

int32_t ucx_task_add(void *task, uint16_t guard_size)
{
	struct tcb_s *tcb_last = kcb_p->tcb_p;
	
	kcb_p->tcb_p = (struct tcb_s *)malloc(sizeof(struct tcb_s));
	if (kcb_p->tcb_first == 0)
		kcb_p->tcb_first = kcb_p->tcb_p;

	if (!kcb_p->tcb_p)
		return -1;

	if (tcb_last)
		tcb_last->tcb_next = kcb_p->tcb_p;
	kcb_p->tcb_p->tcb_next = kcb_p->tcb_first;
	kcb_p->tcb_p->task = task;
	kcb_p->tcb_p->delay = 0;
	kcb_p->tcb_p->guard_sz = guard_size;
	kcb_p->tcb_p->id = kcb_p->id++;
	kcb_p->tcb_p->state = TASK_STOPPED;
	kcb_p->tcb_p->priority = TASK_NORMAL_PRIO;
	kcb_p->tcb_p->p_params = 0;
	printf("add aperiodic task %ld\n", kcb_p->tcb_p->id);
	return 0;
}

int32_t ucx_task_add_periodic(void *task, uint16_t period, uint16_t capacity, uint16_t deadline, uint16_t guard_size)
{
	struct tcb_s *tcb_last = kcb_p->tcb_p;
	struct rt_par_s *p_params = (struct rt_par_s *)malloc(sizeof(struct rt_par_s));
	p_params->period = p_params->rem_period = period;
	p_params->capacity = p_params->rem_capacity = capacity;
	p_params->deadline = p_params->rem_deadline = deadline;
	
	kcb_p->tcb_p = (struct tcb_s *)malloc(sizeof(struct tcb_s));
	if (kcb_p->tcb_first == 0)
		kcb_p->tcb_first = kcb_p->tcb_p;

	if (!kcb_p->tcb_p)
		return -1;

	if (tcb_last)
		tcb_last->tcb_next = kcb_p->tcb_p;
	kcb_p->tcb_p->tcb_next = kcb_p->tcb_first;
	kcb_p->tcb_p->task = task;
	kcb_p->tcb_p->delay = 0;
	kcb_p->tcb_p->guard_sz = guard_size;
	kcb_p->tcb_p->id = kcb_p->id++;
	kcb_p->tcb_p->state = TASK_STOPPED;
	kcb_p->tcb_p->priority = TASK_NORMAL_PRIO;
	/* */
	kcb_p->tcb_p->p_params = p_params;
	printf("add  periodic task %ld\n", kcb_p->tcb_p->id);

	return 0;
}

/*
 * First following lines of code are absurd at best. Stack marks are
 * used by krnl_guard_check() to detect stack overflows on guard space.
 * It is up to the user to define sufficient stack guard space (considering
 * local thread allocation of the stack for recursion and context
 * saving). Stack allocated for data before ucx_task_init() (generally
 * most stack used by a task) is not verified.
 * We also need the safety pig, just in case.
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
	char guard[kcb_p->tcb_p->guard_sz];
	
	memset(guard, 0x69, kcb_p->tcb_p->guard_sz);
	memset(guard, 0x33, 4);
	memset((guard) + kcb_p->tcb_p->guard_sz - 4, 0x33, 4);
	kcb_p->tcb_p->guard_addr = (uint32_t *)guard;
	printf("task %d, guard: %08x - %08x\n", kcb_p->tcb_p->id, (size_t)kcb_p->tcb_p->guard_addr,
		(size_t)kcb_p->tcb_p->guard_addr + kcb_p->tcb_p->guard_sz);
	
	if (!setjmp(kcb_p->tcb_p->context)) {
		kcb_p->tcb_p->state = TASK_READY;
		if (kcb_p->tcb_p->tcb_next == kcb_p->tcb_first) {
			kcb_p->tcb_p->state = TASK_RUNNING;
		} else {
			kcb_p->tcb_p = kcb_p->tcb_p->tcb_next;
			kcb_p->tcb_p->state = TASK_RUNNING;
			(*kcb_p->tcb_p->task)();
		}
	}
	_ei(1);
}

void ucx_task_yield()
{
	if (!setjmp(kcb_p->tcb_p->context)) {
		krnl_delay_update();		/* TODO: check if we need to run a delay update on yields. maybe only on a non-preemtive execution? */ 
		krnl_guard_check();
		krnl_schedule();
		longjmp(kcb_p->tcb_p->context, 1);
	}
}

void ucx_task_delay(uint16_t ticks)
{
	kcb_p->tcb_p->delay = ticks;
	kcb_p->tcb_p->state = TASK_BLOCKED;
	ucx_task_yield();
}

int32_t ucx_task_suspend(uint16_t id)
{
	struct tcb_s *tcb_ptr = kcb_p->tcb_first;
	
	for (;; tcb_ptr = tcb_ptr->tcb_next) {
		if (tcb_ptr->id == id) {
			ucx_critical_enter();
			if (tcb_ptr->state == TASK_READY || tcb_ptr->state == TASK_RUNNING) {
				tcb_ptr->state = TASK_SUSPENDED;
				ucx_critical_leave();
				break;
			} else {
				ucx_critical_leave();
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
			ucx_critical_enter();
			if (tcb_ptr->state == TASK_SUSPENDED) {
				tcb_ptr->state = TASK_READY;
				ucx_critical_leave();
				break;
			} else {
				ucx_critical_leave();
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
			tcb_ptr->priority = priority;
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

uint32_t ucx_context_switch()
{
	return kcb_p->ctx_switches;
}

uint32_t ucx_deadline_misses()
{
	return kcb_p->dln_miss;
}

uint32_t ucx_job_executed()
{
	return kcb_p->job_exec;
}

uint32_t ucx_period_executed()
{
	return kcb_p->prd_exec;
}

void ucx_critical_enter()
{
	_timer_disable();
}

void ucx_critical_leave()
{
	_timer_enable();
}


/* main() function, called from the C runtime */

int32_t main(void)
{
	int32_t pr;
	
	_hardware_init();
	
	kcb_p->tcb_p = 0;
	kcb_p->tcb_first = 0;
	kcb_p->ctx_switches = 0;
	kcb_p->dln_miss = 0;
	kcb_p->job_exec = 0;
	kcb_p->prd_exec = 0;
	kcb_p->id = 0;
	
	printf("UCX/OS boot on %s\n", __ARCH__);
#ifndef UCX_OS_HEAP_SIZE
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&_heap, UCX_OS_HEAP_SIZE);
	printf("heap_init(), %d bytes free\n", UCX_OS_HEAP_SIZE);
#endif
	pr = app_main();
	krnl_sched_init(pr);
	
	return 0;
}
