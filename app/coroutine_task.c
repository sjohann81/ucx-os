#include <ucx.h>

/* application coroutines (executed in a task context) */
struct cgroup_s *cgroup;

void *cr1(void *arg)
{
	static uint32_t count = 0;
	
	printf("hello from corotine 1 [%d]\n", count++);
	
	return 0;
}

void *cr2(void *arg)
{
	static uint32_t count = 0;
	
	printf("hello from corotine 2 [%d]\n", count++);
	
	return 0;
}


/* application tasks */
void task1(void)
{
	int32_t cnt = 200000;
	uint32_t secs, msecs, time;

	while (1) {
		time = ucx_uptime();
		secs = time / 1000;
		msecs = time - secs * 1000;
		printf("[task %d %ld - sys uptime: %ld.%03lds]\n", ucx_task_id(), cnt++, secs, msecs);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_cr_schedule(cgroup, (void *)0);
		ucx_task_wfi();
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_priority(1, TASK_LOW_PRIO);
	
	cgroup = ucx_cr_ginit();
	
	if (!cgroup)
		printf("ucx_cr_ginit() failed!\n");
		
	ucx_cr_add(cgroup, cr1, 5);
	ucx_cr_add(cgroup, cr2, 15);

	// start UCX/OS, preemptive mode
	return 1;
}
