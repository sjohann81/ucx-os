#include <ucx.h>

void task2(void)
{
	int32_t cnt = 300000;
	char guard[256];	/* reserve some guard space. the last thread (task1) may need this! */
	
	ucx_task_init(guard, sizeof(guard));

	while (1) {
		_printf("[task 2 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

void task1(void)
{
	int32_t cnt = 200000;
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		_printf("[task 1 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

void task0(void)
{
	int32_t cnt = 100000;
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		_printf("[task 0 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);

	_printf("hello world!\n");

	// start UCX/OS
	return 0;
}
