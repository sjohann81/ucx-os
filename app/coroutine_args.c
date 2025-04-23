#include <ucx.h>

/* application coroutines (executed in a task context) */
struct cgroup_s *cgroup;

void *cr1(void *arg)
{
	uint32_t *data = (uint32_t *)arg;
	
	printf("hello from corotine 1 [%d]\n", (*data)++);
	
	return 0;
}

void *cr2(void *arg)
{
	uint32_t *data = (uint32_t *)arg;
	
	printf("hello from corotine 2 [%d]\n", (*data)++);
	
	return 0;
}

void *cr3(void *arg)
{
	uint32_t *data = (uint32_t *)arg;
	
	printf("hello from corotine 3 [%d]\n", (*data)++);
	
	return 0;
}

/* only coroutines in this application */
int32_t app_main(void)
{
	uint32_t data = 0;
	
	cgroup = ucx_cr_ginit();
	
	if (!cgroup)
		printf("ucx_cr_ginit() failed!\n");
		
	ucx_cr_add(cgroup, cr1, 5);
	ucx_cr_add(cgroup, cr2, 15);
	ucx_cr_add(cgroup, cr3, 20);

	while (1) {
		ucx_cr_schedule(cgroup, (void *)&data);
	}
}
