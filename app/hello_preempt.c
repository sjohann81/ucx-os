#include <ucx.h>

void task2(void)
{
	int32_t cnt = 300000;

	ucx_task_init();

	while (1) {
		_printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	ucx_task_init();

	while (1) {
		_printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	ucx_task_init();

	while (1) {
		_printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

int32_t app_main(void)
{
	// add tasks, 384 bytes of stack guard space for each
	ucx_task_add(task0, 384);
	ucx_task_add(task1, 384);
	ucx_task_add(task2, 384);

	// start UCX/OS, preemptive mode
	return 1;
}
