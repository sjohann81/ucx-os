#include <ucx.h>

void print_time()
{
	uint32_t secs, msecs, time;
	
	time = ucx_uptime();
	secs = time / 1000;
	msecs = time - secs * 1000;
	
	printf("%ld.%03lds", secs, msecs);
}

/* timer callbacks */
void *timer1(void *arg)
{
	printf("TIMER 1 (");
	print_time();
	printf(")\n");
	
	return 0;
}

void *timer2(void *arg)
{
	printf("TIMER 2 (");
	print_time();
	printf(")\n");
	
	return 0;
}

void *timer3(void *arg)
{
	printf("TIMER 3 (");
	print_time();
	printf(")\n");

	return 0;
}

void *timer4(void *arg)
{
	printf("TIMER 4 (");
	print_time();
	printf(")\n");

	return 0;
}

void *timer5(void *arg)
{
	printf("TIMER 5 (");
	print_time();
	printf(") - one shot!\n");

	return 0;
}

/* software timer task */
void soft_timer(void)
{
	while (1) {
		timer_handler_systick();
	}
}

/* idle task */
void idle(void)
{
	while (1) {
	}
}

int32_t app_main(void)
{
	uint16_t id1, id2, id3, id4, id5;

	id1 = ucx_timer_create(timer1, MS_TO_TICKS(1000));
	id2 = ucx_timer_create(timer2, MS_TO_TICKS(3000));
	id3 = ucx_timer_create(timer3, MS_TO_TICKS(500));
	id4 = ucx_timer_create(timer4, MS_TO_TICKS(10000));
	id5 = ucx_timer_create(timer5, MS_TO_TICKS(5000));
	
	ucx_timer_start(id1, TIMER_AUTORELOAD);
	ucx_timer_start(id2, TIMER_AUTORELOAD);
	ucx_timer_start(id3, TIMER_AUTORELOAD);
	ucx_timer_start(id4, TIMER_AUTORELOAD);
	ucx_timer_start(id5, TIMER_ONESHOT);

	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(soft_timer, DEFAULT_STACK_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
