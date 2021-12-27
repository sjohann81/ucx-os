#include <ucx.h>

void task2(void)
{
	int32_t cnt = 0;
	
	ucx_task_init();

	ucx_task_delay(50);

	while (1) {
		_printf("[task 2 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

void task1(void)
{
	int32_t cnt = 0;

	ucx_task_init();

	while (1) {
		_printf("[task 1 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

void task0(void)
{
	int32_t cnt = 0;

	ucx_task_init();

	while (1) {
		_printf("[task 0 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0, DEFAULT_GUARD_SIZE);
	ucx_task_add(task1, DEFAULT_GUARD_SIZE);
	ucx_task_add(task2, DEFAULT_GUARD_SIZE);

	// start UCX/OS, cooperative mode
	return 0;
}
