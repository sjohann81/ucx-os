/* user data */
#define BUF_SIZE	50

struct my_data_s {
	char buff[BUF_SIZE];
	size_t size;
	int in_use;
};

extern struct device_api_s dev_api;
