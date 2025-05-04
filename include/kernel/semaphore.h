struct sem_s {
	struct queue_s *sem_queue;
	volatile int32_t count;
};

struct sem_s *ucx_sem_create(uint16_t max_tasks, int32_t value);
int32_t ucx_sem_destroy(struct sem_s *s);
void ucx_sem_wait(struct sem_s *s);
int32_t ucx_sem_trywait(struct sem_s *s);
void ucx_sem_signal(struct sem_s *s);
