#include <ucx.h>

#define N_TASKS	2

volatile uint32_t cnt[N_TASKS] = {[0 ... N_TASKS-1] = 0};

void logger(void)
{
	int32_t i;
	
	ucx_task_init();
	
	while (1) {
		_delay_ms(1000);
		for (i = 0; i < N_TASKS; i++)
			printf("%08x ", cnt[i]);
		printf("\n");
	}
}

void task(void)
{
	volatile uint32_t counter = 0;

	ucx_task_init();

	while (1) {
		if (counter++ == 10000) {
			counter = 0;
			cnt[ucx_task_id()]++;
		}
	}
}

int32_t app_main(void)
{
	int32_t i;
	
	for (i = 0; i < N_TASKS; i++)
		ucx_task_add(task, DEFAULT_GUARD_SIZE);
	// add logger task, 384 bytes of stack guard space
	ucx_task_add(logger, DEFAULT_GUARD_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
