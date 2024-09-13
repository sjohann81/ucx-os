#include <ucx.h>

void task2(void)
{
	int32_t cnt = 0;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
	}
}

void task1(void)
{
	int32_t cnt = 0, val;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		if (cnt == 2000) {
			val = ucx_task_resume(2);
			if (val == 0)
				printf("TASK 2 RESUMED!\n");
			else
				printf("FAILED TO RESUME TASK 2\n");
		}
		if (cnt == 6000) {
			val = ucx_task_resume(0);
			if (val == 0)
				printf("TASK 0 RESUMED!\n");
			else
				printf("FAILED TO RESUME TASK 0\n");
		}
	}
}

void task0(void)
{
	int32_t cnt = 0, val;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		if (cnt == 1000) {
			val = ucx_task_suspend(2);
			if (val == 0)
				printf("TASK 2 SUSPENDED!\n");
			else
				printf("FAILED TO SUSPEND TASK 2\n");
		}
		if (cnt == 5000) {
			printf("TRYING TO SUSPEND SELF...");
			ucx_task_suspend(ucx_task_id());
		}
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
