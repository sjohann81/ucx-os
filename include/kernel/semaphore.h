struct sem_s {
	struct queue_s *sem_queue;
	volatile int32_t count;
};

struct sem_s *ucx_semcreate(int32_t value);
int32_t ucx_semdestroy(struct sem_s *s);
void ucx_wait(struct sem_s *s);
void ucx_signal(struct sem_s *s);
