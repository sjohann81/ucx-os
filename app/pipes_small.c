#include <ucx.h>

struct pipe_s *pipe1, *pipe2;

void task2(void)
{
	char data2[64];
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		_sprintf(data2, "Hello from task 2!");
		/* write pipe - write size must be less than buffer size */
		pipe_write(pipe2, data2, _strlen((char *)data2));
	}
}

void task1(void)
{
	char data1[64];
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		_sprintf(data1, "Hello from task 1!");
		/* write pipe - write size must be less than buffer size */
		pipe_write(pipe1, data1, _strlen((char *)data1));
	}
}

void task0(void)
{
	char data[64];
	uint16_t s;
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* read pipe - read size must be less than buffer size */
		s = pipe_read(pipe1, data, 63);
		_printf("pipe (%d): %s\n", s, data);
		s = pipe_read(pipe2, data, 31);
		_printf("pipe (%d): %s\n", s, data);
		
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);

	pipe1 = pipe_create(64);		/* pipe buffer, 64 bytes - powers of two */
	pipe2 = pipe_create(32);		/* pipe buffer, 32 bytes - powers of two */

	// start UCX/OS, preemptive mode
	return 1;
}
