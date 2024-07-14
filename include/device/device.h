enum device_mode {O_RDONLY, O_WRONLY, O_RDWR};

/* device data structure */
struct device_s {
	const char *name;
	const void *config;
	const void *data;
	const struct device_api_s *api;
	const void *custom_api;
};

/* generic device API */
struct device_api_s {
	int (*dev_init)(const struct device_s *dev);
	int (*dev_open)(const struct device_s *dev, int mode);
	int (*dev_close)(const struct device_s *dev);
	size_t (*dev_read)(const struct device_s *dev, void *buf, size_t count);
	size_t (*dev_write)(const struct device_s *dev, void *buf, size_t count);
};
