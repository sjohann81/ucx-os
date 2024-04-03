/* file:          event.c
 * description:   event management
 * date:          11/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct eq_s *ucx_eq_create(uint16_t events)
{
	struct eq_s *eqptr;
	
	eqptr = malloc(sizeof(struct eq_s));
	
	if (!eqptr)
		return 0;
		
	eqptr->event_queue = queue_create(events);
	
	if (!eqptr->event_queue) {
		free(eqptr);
		return 0;
	}
	
	eqptr->mutex = ucx_sem_create(EQ_SEM_MAX_TASKS, 1);
	
	if (!eqptr->mutex) {
		queue_destroy(eqptr->event_queue);
		free(eqptr);
		return 0;
	}
	
	return eqptr;
}

int32_t ucx_eq_destroy(struct eq_s *eq)
{
	ucx_sem_wait(eq->mutex);
	
	if (queue_count(eq->event_queue)) {
		ucx_sem_signal(eq->mutex);
		
		return ERR_EQ_NOTEMPTY;
	}
	
	queue_destroy(eq->event_queue);
	
	if (ucx_sem_destroy(eq->mutex))
		return ERR_SEM_DEALLOC;
		
	free(eq);
	
	return 0;
}

int32_t ucx_event_post(struct eq_s *eq, struct event_s *e)
{
	int32_t status;
	
	ucx_sem_wait(eq->mutex);
	status = queue_enqueue(eq->event_queue, e);
	ucx_sem_signal(eq->mutex);
	
	return status;
}

int32_t ucx_event_poll(struct eq_s *eq)
{
	int32_t ecount;
	
	ucx_sem_wait(eq->mutex);
	ecount = queue_count(eq->event_queue);
	ucx_sem_signal(eq->mutex);
	
	return ecount;
} 

struct event_s *ucx_event_get(struct eq_s *eq)
{
	struct event_s *e;
	
	ucx_sem_wait(eq->mutex);
	e = queue_dequeue(eq->event_queue);
	ucx_sem_signal(eq->mutex);
	
	return e;
}

void *ucx_event_dispatch(struct event_s *e, void *arg)
{
	if (e->callback)
		return e->callback(arg);
	else
		return 0;
}
