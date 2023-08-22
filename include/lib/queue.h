struct queue_s {
	void **pdata;				/* pointer to an array of pointer elements */
	int32_t size;				/* must be a power of 2 */
	int32_t mask;
	int32_t head, tail, elem;
};

struct queue_s *queue_create(int32_t size);
int32_t queue_destroy(struct queue_s *q);
int32_t queue_count(struct queue_s *q);
int32_t queue_enqueue(struct queue_s *q, void *ptr);
void *queue_dequeue(struct queue_s *q);
void *queue_peek(struct queue_s *q);
