struct queue_s {
	void **pdata;				/* pointer to an array of pointer elements */
	int32_t size;				/* must be a power of 2 */
	int32_t mask;
	int32_t head, tail, elem;
};

struct queue_s *ucx_queue_create(int32_t size);
int32_t ucx_queue_destroy(struct queue_s *q);
int32_t ucx_queue_count(struct queue_s *q);
int32_t ucx_queue_enqueue(struct queue_s *q, void *ptr);
void *ucx_queue_dequeue(struct queue_s *q);
void *ucx_queue_peek(struct queue_s *q);
