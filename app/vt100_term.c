#include <ucx.h>
#include <vt100.h>

/* device driver instantiation */
struct device_s vt100_device = {
	.name = "vt100",
	.custom_api = &vt100_api
};

void task2(void)
{
	int32_t cnt = 300000;

	while (1) {
		vt100_textattr(&vt100_device, TEXT_RESET);
		vt100_textattr(&vt100_device, TEXT_GREEN);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	while (1) {
		vt100_textattr(&vt100_device, TEXT_RESET);
		vt100_textattr(&vt100_device, TEXT_CYAN);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		vt100_textattr(&vt100_device, TEXT_RESET);
		vt100_textattr(&vt100_device, TEXT_MAGENTA);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0, DEFAULT_STACK_SIZE);
	ucx_task_add(task1, DEFAULT_STACK_SIZE);
	ucx_task_add(task2, DEFAULT_STACK_SIZE);
	
	ucx_task_priority(2, TASK_LOW_PRIO);
	
	vt100_term(&vt100_device, TERM_RESET);

	// start UCX/OS, preemptive mode
	return 1;
}
