#include <ucx.h>
#include <device.h>
#include "driver.h"

/* device driver function implementation */
static int my_init(const struct device_s *dev)
{
	struct my_data_s *pdata;
	
	printf("DEV: device init\n");
	
	CRITICAL_ENTER();
	pdata = (struct my_data_s *)dev->data;
	pdata->buf = 0;
	pdata->size = 0;
	pdata->mode = 0;
	pdata->in_use = -1;
	CRITICAL_LEAVE();

	return 0;
}

static int my_open(const struct device_s *dev, int mode)
{
	struct my_data_s *pdata;
	int retval = 0;

	NOSCHED_ENTER();
	pdata = (struct my_data_s *)dev->data;

	pdata->mode = mode;

	if (pdata->in_use == -1) {
		pdata->in_use = ucx_task_id();
		
		if (pdata->mode)
			printf("DEV: device open (task %d)\n", pdata->in_use);
	} else {
		if (pdata->mode)
			printf("DEV: device open failed\n");

		retval = -1;
	}
	NOSCHED_LEAVE();

	return retval;
}

static int my_close(const struct device_s *dev)
{
	struct my_data_s *pdata;
	int retval = 0;

	NOSCHED_ENTER();
	pdata = (struct my_data_s *)dev->data;

	if (pdata->in_use > -1) {
		if (pdata->mode)
			printf("DEV: device close (task %d)\n", pdata->in_use);
		
		pdata->mode = 0;
		pdata->in_use = -1;
	} else {
		if (pdata->mode)
			printf("DEV: device close failed\n");
		
		retval = -1;
	}
	NOSCHED_LEAVE();

	return retval;
}

static size_t my_read(const struct device_s *dev, void *buf, size_t count)
{
	struct my_data_s *pdata;
	
	NOSCHED_ENTER();
	pdata = (struct my_data_s *)dev->data;
	
	if (pdata->in_use != ucx_task_id()) {
		NOSCHED_LEAVE();
		
		return -1;
	}

	if (count < pdata->size)
		pdata->size = count;
	else
		count = pdata->size;
	
	if (pdata->size) {
		memcpy(buf, pdata->buf, pdata->size);
		pdata->size = 0;
		free(pdata->buf);
		pdata->buf = 0;
	}
	NOSCHED_LEAVE();
	
	if (pdata->mode)
		printf("DEV: device read %d bytes\n", count);
	
	return count;
}

static size_t my_write(const struct device_s *dev, void *buf, size_t count)
{
	struct my_data_s *pdata;

	NOSCHED_ENTER();
	pdata = (struct my_data_s *)dev->data;
	
	if (pdata->in_use != ucx_task_id()) {
		NOSCHED_LEAVE();
		
		return -1;
	}

	if (!pdata->buf) {
		pdata->buf = malloc(count);
		if (!pdata->buf) {
			printf("DEV: out of memory.\n");
			
			return -1;
		}
		
		memcpy(pdata->buf, buf, count);
		pdata->size = count;
		
		if (pdata->mode)
			printf("DEV: device write %d bytes\n", count);
	} else {
		if (pdata->mode)
			printf("DEV: device drop %d bytes\n", count);
	}
	NOSCHED_LEAVE();
	
	return pdata->size;
}

/* device driver function mapping for generic API */
struct device_api_s dev_api = {
	.dev_init = my_init,
	.dev_open = my_open,
	.dev_close = my_close,
	.dev_read = my_read,
	.dev_write = my_write
};
