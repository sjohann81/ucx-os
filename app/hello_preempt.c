#include <ucx.h>

void task2(void)
{
	int32_t cnt = 300000;

	ucx_task_init();

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	ucx_task_init();

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	ucx_task_init();

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0, DEFAULT_GUARD_SIZE);
	ucx_task_add(task1, DEFAULT_GUARD_SIZE);
	ucx_task_add(task2, DEFAULT_GUARD_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
