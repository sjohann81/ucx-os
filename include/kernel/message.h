struct message_s {
	void *data;
	uint16_t type;
	uint16_t size;
};

struct mq_s {
	struct queue_s *queue;
};

struct mq_s *ucx_mq_create(uint16_t size);
int32_t ucx_mq_destroy(struct mq_s *mq);
int32_t ucx_mq_enqueue(struct mq_s *mq, struct message_s *m);
struct message_s *ucx_mq_dequeue(struct mq_s *mq);
struct message_s *ucx_mq_peek(struct mq_s *mq);
int32_t ucx_mq_items(struct mq_s *mq);
