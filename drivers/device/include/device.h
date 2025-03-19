#include <hal.h>

enum device_mode {O_RDONLY, O_WRONLY, O_RDWR};
enum seek_pos {SEEK_SET, SEEK_CUR, SEEK_END};

/* device data structure */
struct device_s {
	const char *name;
	const void *config;
	void *data;
	const struct device_api_s *api;
	const void *custom_api;
};

/* generic driver API */
int dev_init(const struct device_s *dev);
int dev_deinit(const struct device_s *dev);
int dev_open(const struct device_s *dev, int mode);
int dev_close(const struct device_s *dev);
size_t dev_read(const struct device_s *dev, void *buf, size_t count);
size_t dev_write(const struct device_s *dev, void *buf, size_t count);
int dev_seek(const struct device_s *dev, long pos, int whence);
int dev_ioctl(const struct device_s *dev, unsigned int req, ...);

/* generic driver interface */
struct device_api_s {
	int (*dev_init)(const struct device_s *dev);
	int (*dev_deinit)(const struct device_s *dev);
	int (*dev_open)(const struct device_s *dev, int mode);
	int (*dev_close)(const struct device_s *dev);
	size_t (*dev_read)(const struct device_s *dev, void *buf, size_t count);
	size_t (*dev_write)(const struct device_s *dev, void *buf, size_t count);
	int (*dev_seek)(const struct device_s *dev, long pos, int whence);
	int (*dev_ioctl)(const struct device_s *dev, unsigned int req, ...);
};
