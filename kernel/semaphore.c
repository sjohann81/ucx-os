/* file:          semaphore.c
 * description:   semaphore implementation
 * date:          11/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct sem_s *ucx_semcreate(int32_t value)
{
	struct sem_s *s;
	
	s = (struct sem_s *)malloc(sizeof(struct sem_s));
	
	if (!s)
		return 0;
	
	s->sem_queue = ucx_queue_create(ucx_task_count());
	
	if ((!s->sem_queue) || (value < 0)) {
		free(s);
		
		return 0;
	} else {
		s->count = value;

		return s;
	}
}

int32_t ucx_semdestroy(struct sem_s *s)
{
	
	if (ucx_queue_destroy(s->sem_queue)) {
		return -1;
	} else {
		free(s);
		
		return 0;
	}
}

void ucx_wait(struct sem_s *s)
{
	extern struct kcb_s *kcb_p;
	
	CRITICAL_ENTER();
	s->count--;
	if (s->count < 0) {
		ucx_queue_enqueue(s->sem_queue, kcb_p->tcb_p);
		kcb_p->tcb_p->state = TASK_BLOCKED;
		CRITICAL_LEAVE();
		ucx_task_wfi();
	} else {
		CRITICAL_LEAVE();
	}
}

void ucx_signal(struct sem_s *s)
{
	struct tcb_s *tcb_sem; 
	
	CRITICAL_ENTER();
	s->count++;
	if (s->count <= 0) {
		/* FIXME: unblock the task with the highest priority instead of the first one */
		tcb_sem = ucx_queue_dequeue(s->sem_queue);
		tcb_sem->state = TASK_READY;
	}
	CRITICAL_LEAVE();
}
