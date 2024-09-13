#include <ucx.h>

#define N_TASKS	2

volatile uint32_t cnt[N_TASKS] = {[0 ... N_TASKS-1] = 0};

void logger(void)
{
	int32_t i;
	uint32_t ms;
	
	while (1) {
		ms = _read_us() / 1000;
		_delay_ms(1000);
		ms = _read_us() / 1000 - ms;
		for (i = 0; i < N_TASKS; i++)
			printf("%08x ", cnt[i]);
		printf("ms: %d (total: %d)\n", ms, _read_us() / 1000);
	}
}

void task(void)
{
	volatile uint32_t counter = 0;

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
		ucx_task_spawn(task, DEFAULT_STACK_SIZE);
	// add logger task
	ucx_task_spawn(logger, DEFAULT_STACK_SIZE);

	// start UCX/OS, preemptive mode
	return 1;
}
