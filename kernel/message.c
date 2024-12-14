/* file:          message.c
 * description:   message queue
 * date:          09/2024
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct mq_s *ucx_mq_create(uint16_t size)
{
	struct mq_s *mqptr;
	
	mqptr = malloc(sizeof(struct mq_s));
	
	if (!mqptr)
		return 0;
		
	mqptr->queue = queue_create(size);
	
	if (!mqptr->queue) {
		free(mqptr);
		return 0;
	}
	
	return mqptr;
}

int32_t ucx_mq_destroy(struct mq_s *mq)
{
	if (queue_count(mq->queue))
		return ERR_MQ_NOTEMPTY;
	
	CRITICAL_ENTER();
	queue_destroy(mq->queue);
	free(mq);
	CRITICAL_LEAVE();
	
	return 0;
}

int32_t ucx_mq_enqueue(struct mq_s *mq, struct message_s *m)
{
	int32_t status;
	
	CRITICAL_ENTER();
	status = queue_enqueue(mq->queue, m);
	CRITICAL_LEAVE();
	
	return status;
}

struct message_s *ucx_mq_dequeue(struct mq_s *mq)
{
	struct message_s *m;
	
	CRITICAL_ENTER();
	m = queue_dequeue(mq->queue);
	CRITICAL_LEAVE();
	
	return m;
}

struct message_s *ucx_mq_peek(struct mq_s *mq)
{
	struct message_s *m;
	
	CRITICAL_ENTER();
	m = queue_peek(mq->queue);
	CRITICAL_LEAVE();
	
	return m;
}

int32_t ucx_mq_items(struct mq_s *mq)
{
	int32_t mcount;
	
	mcount = queue_count(mq->queue);
	
	return mcount;
}
