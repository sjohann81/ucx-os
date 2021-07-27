#include <ucx.h>

#define N_TASKS	3

volatile uint32_t cnt[N_TASKS] = {[0 ... N_TASKS-1] = 0};

void logger(void)
{
	int32_t i;
	char guard[256];
	
	ucx_task_init(guard, sizeof(guard));
	
	while (1) {
		_delay_ms(1000);
		for (i = 0; i < N_TASKS; i++)
			_printf("%08x ", cnt[i]);
		_printf("\n");
	}
}

void task(void)
{
	volatile uint32_t counter = 0;
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

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
		ucx_task_add(task);
	ucx_task_add(logger);

	// start UCX/OS, preemptive mode
	return 1;
}
