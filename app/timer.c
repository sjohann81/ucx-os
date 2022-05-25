#include <ucx.h>

void timer1(void)
{
	ucx_task_init();

	while (1) {
		printf("TIMER 1\n");
		ucx_task_delay(100);
	}
}

void timer2(void)
{
	ucx_task_init();

	while (1) {
		printf("TIMER 2\n");
		ucx_task_delay(300);
	}
}

void timer3(void)
{
	ucx_task_init();

	while (1) {
		printf("TIMER 3\n");
		ucx_task_delay(50);
	}
}

void idle(void)
{
	ucx_task_init();
	
	while (1)
	{
	}
}

int32_t app_main(void)
{
	ucx_task_add(timer1, DEFAULT_GUARD_SIZE);
	ucx_task_add(timer2, DEFAULT_GUARD_SIZE);
	ucx_task_add(timer3, DEFAULT_GUARD_SIZE);
	ucx_task_add(idle, DEFAULT_GUARD_SIZE);

	// start UCX/OS, cooperative mode
	return 1;
}
