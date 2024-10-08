/* file:          message.c
 * description:   message queue
 * date:          09/2024
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct mq_s *ucx_mq_create(uint16_t events)
{
	struct mq_s *mqptr;
	
	mqptr = malloc(sizeof(struct mq_s));
	
	if (!mqptr)
		return 0;
		
	mqptr->queue = queue_create(events);
	
	if (!mqptr->queue) {
		free(mqptr);
		return 0;
	}
	
	mqptr->mutex = ucx_sem_create(EQ_SEM_MAX_TASKS, 1);
	
	if (!mqptr->mutex) {
		queue_destroy(mqptr->queue);
		free(mqptr);
		
		return 0;
	}
	
	return mqptr;
}

int32_t ucx_mq_destroy(struct mq_s *mq)
{
	ucx_sem_wait(mq->mutex);
	
	if (queue_count(mq->queue)) {
		ucx_sem_signal(mq->mutex);
		
		return ERR_EQ_NOTEMPTY;
	}
	
	queue_destroy(mq->queue);
	
	if (ucx_sem_destroy(mq->mutex))
		return ERR_SEM_DEALLOC;
		
	free(mq);
	
	return 0;
}

int32_t ucx_mq_enqueue(struct mq_s *mq, struct message_s *m)
{
	int32_t status;
	
	ucx_sem_wait(mq->mutex);
	status = queue_enqueue(mq->queue, m);
	ucx_sem_signal(mq->mutex);
	
	return status;
}

struct message_s *ucx_mq_dequeue(struct mq_s *mq)
{
	struct message_s *m;
	
	ucx_sem_wait(mq->mutex);
	m = queue_dequeue(mq->queue);
	ucx_sem_signal(mq->mutex);
	
	return m;
}

int32_t ucx_mq_items(struct mq_s *mq)
{
	int32_t mcount;
	
	mcount = queue_count(mq->queue);
	
	return mcount;
}
