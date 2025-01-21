#include <ucx.h>

void print_time()
{
	uint32_t secs, msecs, time;
	
	time = ucx_uptime();
	secs = time / 1000;
	msecs = time - secs * 1000;
	
	printf("%ld.%03lds", secs, msecs);
}

void timer1(void)
{
	while (1) {
		printf("TIMER 1 (");
		print_time();
		printf(")\n");
		ucx_task_delay(MS_TO_TICKS(1000));
	}
}

void timer2(void)
{
	while (1) {
		printf("TIMER 2 (");
		print_time();
		printf(")\n");
		ucx_task_delay(MS_TO_TICKS(3000));
	}
}

void timer3(void)
{
	while (1) {
		printf("TIMER 3 (");
		print_time();
		printf(")\n");
		ucx_task_delay(MS_TO_TICKS(500));
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
