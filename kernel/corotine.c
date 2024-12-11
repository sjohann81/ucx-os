#include <ucx.h>

/* initialize a corotine group */
int32_t ucx_corotine_init(struct list_s *cgroup)
{
	return 0;
}

/* add a corotine to a group */
int32_t ucx_corotine_add(struct list_s *cgroup, void *(corotine)(void *), uint8_t priority)
{
	return 0;
}

/* remove a corotine from a group */
int32_t ucx_corotine_cancel(struct list_s *cgroup, void *(corotine)(void *))
{
	return 0;
}

/* schedule a group of corotines */
int32_t ucx_corotine_schedule(struct list_s *cgroup)
{
	return 0;
}
