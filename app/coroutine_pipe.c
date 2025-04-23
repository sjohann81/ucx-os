#include <ucx.h>

struct pipe_s *pipe1, *pipe2;

void *cr2(void *arg)
{
	char data2[64];

	sprintf(data2, "Hello from task 2!");
	/* write pipe - write size must be less than buffer size */
	ucx_pipe_nbwrite(pipe2, data2, strlen((char *)data2));

	return 0;
}

void *cr1(void *arg)
{
	char data1[64];

	sprintf(data1, "Hello from task 1!");
	/* write pipe - write size must be less than buffer size */
	ucx_pipe_nbwrite(pipe1, data1, strlen((char *)data1));

	return 0;
}

void *cr0(void *arg)
{
	char data[64];
	uint16_t s;

	/* read pipe - read size must be less than buffer size */
	memset(data, 0, sizeof(data));
	s = ucx_pipe_nbread(pipe1, data, 63);
	printf("pipe (%d): %s\n", s, data);
	memset(data, 0, sizeof(data));
	s = ucx_pipe_nbread(pipe2, data, 50);
	printf("pipe (%d): %s\n", s, data);
		
	return 0;
}

int32_t app_main(void)
{
	struct cgroup_s *cgroup;
	
	cgroup = ucx_cr_ginit();
	ucx_cr_add(cgroup, cr0, 50);
	ucx_cr_add(cgroup, cr1, 50);
	ucx_cr_add(cgroup, cr2, 50);

	pipe1 = ucx_pipe_create(64);		/* pipe buffer, 64 bytes (allocated from the heap) */
	pipe2 = ucx_pipe_create(32);		/* pipe buffer, 32 bytes (allocated from the heap) */

	while (1) {
		ucx_cr_schedule(cgroup, (void *)0);
	}
}
