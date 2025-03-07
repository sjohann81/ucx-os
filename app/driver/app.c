#include <ucx.h>
#include <device.h>
#include "driver.h"

struct my_data_s my_data1, my_data2;

/* device driver instantiation */
const struct device_s device1 = {
	.name = "device 1",
	.data = &my_data1,
	.api = &dev_api
};

const struct device_s device2 = {
	.name = "device 2",
	.data = &my_data2,
	.api = &dev_api
};

/* user tasks */
void task0(void)
{
	const struct device_s *dev = &device1;
	char buf[BUF_SIZE];
	int i = 0;
	
	while (1) {
		if (dev->api->dev_open(dev, 0)) {
			ucx_task_delay(MS_TO_TICKS(100));
			continue;
		}
		sprintf(buf, "hello world! %d", i++);
		dev->api->dev_write(dev, buf, strlen(buf) + 1);
		dev->api->dev_close(dev);
		
		ucx_task_delay(MS_TO_TICKS(1000));
	}
}

void task1(void)
{
	const struct device_s *dev = &device1;
	char buf[BUF_SIZE];
	size_t size;
	
	while (1) {
		if (dev->api->dev_open(dev, 0)) {
			ucx_task_delay(MS_TO_TICKS(100));
			continue;
		}

		size = dev->api->dev_read(dev, buf, BUF_SIZE);
		if (size)
			printf("data (%d): %s\n", size, buf);
		dev->api->dev_close(dev);
		
		ucx_task_delay(MS_TO_TICKS(1000));
	}
}

void idle(void)
{
	while (1) {
	}
}


/* application entry point */
int32_t app_main(void)
{
	const struct device_s *dev1 = &device1;
	const struct device_s *dev2 = &device2;
	
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_priority(0, TASK_LOW_PRIO);
	dev1->api->dev_init(dev1);
	dev2->api->dev_init(dev2);

	// start UCX/OS
	return 1;
}
