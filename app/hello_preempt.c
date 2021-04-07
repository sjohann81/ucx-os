#include <ucx.h>

void task2(void)
{
	char guard[512];	/* reserve some guard space. the last thread (task1) may need this! */
	int32_t cnt = 300000;

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		printf("[task 2 %d]\n", cnt++);
	}
}

void task1(void)
{
	char guard[512];
	int32_t cnt = 200000;

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		printf("[task 1 %d]\n", cnt++);
	}
}

void task0(void)
{
	char guard[512];
	int32_t cnt = 100000;

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		printf("[task 0 %d]\n", cnt++);
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);

	printf("hello world!\n");

	// start UCX/OS, preemptive mode
	return 1;
}
