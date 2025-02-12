#include <ucx.h>

void task2(void)
{
	int32_t cnt = 300000;
	uint32_t secs, msecs, time;

	while (1) {
		time = ucx_uptime();
		secs = time / 1000;
		msecs = time - secs * 1000;
		printf("[task %d %ld - sys uptime: %ld.%03lds]\n", ucx_task_id(), cnt++, secs, msecs);
		ucx_task_wfi();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	
	ucx_task_priority(2, TASK_LOW_PRIO);
	
	printf("task0 has id %d\n", ucx_task_idref(task0));
	printf("task1 has id %d\n", ucx_task_idref(task1));
	printf("task2 has id %d\n", ucx_task_idref(task2));

	// start UCX/OS, preemptive mode
	return 1;
}
