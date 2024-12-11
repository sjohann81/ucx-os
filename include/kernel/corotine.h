/* corotine control block */
struct ccb_s {
	void *(*corotine)(void *);
	uint8_t priority;
	uint8_t pcounter;
};

/* corotine management API */
int32_t ucx_corotine_init(struct list_s *cgroup);
int32_t ucx_corotine_add(struct list_s *cgroup, void *(corotine)(void *), uint8_t priority);
int32_t ucx_corotine_cancel(struct list_s *cgroup, void *(corotine)(void *));
int32_t ucx_corotine_schedule(struct list_s *cgroup);
