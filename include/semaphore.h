struct sem_s {
	struct queue_s *sem_queue;
	volatile int32_t count;
};

int32_t ucx_seminit(struct sem_s *s, int32_t value);
int32_t ucx_semdestroy(struct sem_s *s);
void ucx_wait(struct sem_s *s);
void ucx_signal(struct sem_s *s);
