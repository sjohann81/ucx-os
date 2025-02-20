#include <device.h>

/* generic driver API function wrappers */
int dev_init(const struct device_s *dev)
{
	return dev->api->dev_init(dev);
}

int dev_deinit(const struct device_s *dev)
{
	return dev->api->dev_deinit(dev);
}

int dev_open(const struct device_s *dev, int mode)
{
	return dev->api->dev_open(dev, mode);
}

int dev_close(const struct device_s *dev)
{
	return dev->api->dev_close(dev);
}

size_t dev_read(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_read(dev, buf, count);
}

size_t dev_write(const struct device_s *dev, void *buf, size_t count)
{
	return dev->api->dev_write(dev, buf, count);
}

int dev_ioctl(const struct device_s *dev, int cmd, void *data)
{
	return dev->api->dev_ioctl(dev, cmd, data);
}
