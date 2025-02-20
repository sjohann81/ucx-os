#include <ucx.h>
#include <device.h>
#include <vt100.h>

/* device driver instantiation */
const struct device_s vt100_device = {
	.name = "vt100",
	.custom_api = &vt100_api
};

const struct device_s *vt100 = &vt100_device;
const struct vt100_api_s *vt100_dev_api = (const struct vt100_api_s *)(&vt100_device)->custom_api;

void task2(void)
{
	int32_t cnt = 300000;

	while (1) {
		vt100_dev_api->textattr(vt100, TEXT_RESET);
		vt100_dev_api->textattr(vt100, TEXT_GREEN);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	while (1) {
		vt100_dev_api->textattr(vt100, TEXT_RESET);
		vt100_dev_api->textattr(vt100, TEXT_CYAN);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		vt100_dev_api->textattr(vt100, TEXT_RESET);
		vt100_dev_api->textattr(vt100, TEXT_MAGENTA);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	
	ucx_task_priority(2, TASK_LOW_PRIO);
	
	vt100_dev_api->term(vt100, TERM_RESET);

	// start UCX/OS, preemptive mode
	return 1;
}
