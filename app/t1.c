#include <ucx.h>

void task_periodic(void)
{
	ucx_task_init();

	while (1) {
		printf("[task periodic %ld]\n", ucx_task_id());
		ucx_task_wfi();
	}
}

void task_idle(void)
{
	int32_t cnt = 0;
	ucx_task_init();

	while (1) {
		printf("[task idle %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task_metrics(void)
{
	ucx_task_init();

	while (1)
	{
		printf("[task metrics %d dln_miss:%ld job_exec:%ld prd_exec:%ld ctx_switch:%ld]\n", ucx_task_id(), ucx_deadline_misses(), ucx_job_executed(), ucx_period_executed(), ucx_context_switch() - 1);
		ucx_task_wfi();
	}
	
}

int32_t app_main(void)
{
	ucx_task_add_periodic(task_periodic, 20, 2, 20, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task_periodic, 7, 1, 7, DEFAULT_GUARD_SIZE);
	ucx_task_add_periodic(task_periodic, 10, 2, 10, DEFAULT_GUARD_SIZE);
	ucx_task_add(task_metrics, DEFAULT_GUARD_SIZE);
	ucx_task_add(task_idle, DEFAULT_GUARD_SIZE);
	
	// start UCX/OS, preemptive mode
	return 1;
}