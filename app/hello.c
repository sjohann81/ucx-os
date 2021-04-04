#include <ucx.h>

void task4(void)
{
	char stack[512];

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		printf(".");
	}
}

void task3(void)
{
	char stack[512];	/* reserve some stack space */
	int32_t cnt = 400000;

	ucx_task_init(stack, sizeof(stack));

	while (1) {			/* thread body */
		printf("[task 3 %d]\n", cnt++);
		ucx_task_wfi();		/* wait for the next context switch */
	}
}

void task2(void)
{
	char stack[512];
	int32_t cnt = 300000;

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		printf("[task 2 %d]\n", cnt++);
		ucx_task_wfi();
	}
}

void task1(void)
{
	char stack[512];
	int32_t cnt = 200000;

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		printf("[task 1 %d]\n", cnt++);
		ucx_task_wfi();
	}
}

void task0(void)
{
	char stack[512];
	int32_t cnt = 100000;

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		printf("[task 0 %d]\n", cnt++);
		ucx_task_wfi();
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);
	ucx_task_add(task3);
	ucx_task_add(task4);

	printf("hello world!\n");

	return 0;
}
