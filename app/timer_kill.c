#include <ucx.h>

void timer1(void)
{
	while (1) {
		printf("T%d, TIMER 1\n", ucx_task_id());
		ucx_task_delay(100);
	}
}

void timer2(void)
{
	int i = 0;
	
	while (1) {
		printf("T%d, TIMER 2\n", ucx_task_id());
		ucx_task_delay(300);
		if (++i == 10) {
			printf("killing task 3...\n");
			ucx_task_cancel(2);
		}
	}
}

void timer3(void)
{
	while (1) {
		printf("T%d, TIMER 3\n", ucx_task_id());
		ucx_task_delay(50);
	}
}

void idle(void)
{
	while (1) {
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(timer1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(timer2, DEFAULT_STACK_SIZE);
	ucx_task_spawn(timer3, DEFAULT_STACK_SIZE);
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
