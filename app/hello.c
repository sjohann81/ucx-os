#include <ucx.h>

void task2(void)
{
	int32_t cnt = 300000;
	
	ucx_task_init();

	while (1) {
		_printf("[task 2 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	ucx_task_init();

	while (1) {
		_printf("[task 1 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	ucx_task_init();

	while (1) {
		_printf("[task 0 %ld]\n", cnt++);
		ucx_task_yield();
	}
}

int32_t app_main(void)
{
	// add tasks, 384 bytes of stack guard space for each
	ucx_task_add(task0, 384);
	ucx_task_add(task1, 384);
	ucx_task_add(task2, 384);

	_printf("hello world!\n");

	// start UCX/OS, cooperative mode
	return 0;
}
