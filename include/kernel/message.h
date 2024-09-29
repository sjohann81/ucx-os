#define MQ_SEM_MAX_TASKS	16

struct message_s {
	void *data;
	uint16_t type;
};

struct mq_s {
	struct queue_s *queue;
	struct sem_s *mutex;
};

struct mq_s *ucx_mq_create(uint16_t events);
int32_t ucx_mq_destroy(struct mq_s *mq);
int32_t ucx_mq_enqueue(struct mq_s *mq, struct message_s *m);
struct message_s *ucx_mq_dequeue(struct mq_s *mq);
int32_t ucx_mq_items(struct mq_s *mq);
