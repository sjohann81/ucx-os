/* corotine control block */
struct ccb_s {
	void *(*corotine)(void *);
	uint8_t priority;
	uint8_t pcounter;
};

struct cgroup_s {
	struct list_s *crlist;
	uint16_t fibers;
};

/* corotine management API */
struct cgroup_s *ucx_cr_ginit(void);
int32_t ucx_cr_gdestroy(struct cgroup_s *cgroup);
int32_t ucx_cr_add(struct cgroup_s *cgroup, void *(corotine)(void *), uint8_t priority);
int32_t ucx_cr_cancel(struct cgroup_s *cgroup, void *(corotine)(void *));
int32_t ucx_cr_schedule(struct cgroup_s *cgroup, void *arg);
