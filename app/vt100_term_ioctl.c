#include <ucx.h>
#include <device.h>
#include <vt100.h>

/* device driver instantiation */
const struct device_s vt100_device = {
	.name = "vt100",
	.api = &vt100_api_generic
};

const struct device_s *vt100 = &vt100_device;

void task2(void)
{
	int32_t cnt = 300000;

	while (1) {
		vt100->api->dev_ioctl(vt100, TEXTATTR, TEXT_RESET);
		vt100->api->dev_ioctl(vt100, TEXTATTR, TEXT_GREEN);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	while (1) {
		vt100->api->dev_ioctl(vt100, TEXTATTR, TEXT_RESET);
		vt100->api->dev_ioctl(vt100, TEXTATTR, TEXT_CYAN);
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		vt100->api->dev_ioctl(vt100, TEXTATTR, TEXT_RESET);
		vt100->api->dev_ioctl(vt100, TEXTATTR, TEXT_MAGENTA);
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
	
	vt100->api->dev_ioctl(vt100, TERM, TERM_RESET);
	vt100->api->dev_ioctl(vt100, CURSORPOS, 10, 25);
	printf("hey!!\n");
	_delay_ms(2000);
	vt100->api->dev_ioctl(vt100, TERM, TERM_RESET);

	// start UCX/OS, preemptive mode
	return 1;
}
