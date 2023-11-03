#define EQ_SEM_MAX_TASKS	16

struct event_s {
	void *(*callback)(void *);
	void *data;
	uint16_t type;
};

struct eq_s {
	struct queue_s *event_queue;
	struct sem_s *mutex;
};

struct eq_s *ucx_eq_create(uint16_t events);
int32_t ucx_eq_destroy(struct eq_s *eq);
int32_t ucx_event_post(struct eq_s *eq, struct event_s *e);
int32_t ucx_event_poll(struct eq_s *eq);
struct event_s *ucx_event_get(struct eq_s *eq);
void *ucx_event_dispatch(struct event_s *e, void *arg);
