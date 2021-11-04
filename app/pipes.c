#include <ucx.h>

struct pipe_s *pipe1, *pipe2, *pipe3;

char data1[128] = "hello from task 1!";
char data2[128] = "heya from task 2!";
char hello[64] = "hi!";

void task3(void)
{
	char data[128];
	uint16_t s;
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		_memset(data, 0, sizeof(data));
		_printf("Waiting data from task0... ");
		/* read pipe - read size must be less than buffer size */
		s = ucx_pipe_read(pipe3, data, 127);
		_printf("pipe 3 (%d): %s\n", s, data);
	}
}

void task2(void)
{
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		ucx_pipe_write(pipe2, data2, _strlen(data2));
	}
}

void task1(void)
{
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		ucx_pipe_write(pipe1, data1, _strlen(data1));
	}
}

void task0(void)
{
	char data1[128];	/* data buffer 1 */
	char data2[50];		/* data buffer 2 */
	uint16_t s;
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* read pipe - read size must be less than buffer size */
		s = ucx_pipe_read(pipe1, data1, 127);
		_printf("pipe 1 (%d): %s\n", s, data1);
		s = ucx_pipe_read(pipe2, data2, 10);
		_printf("pipe 2 (%d): %s\n", s, data2);
		
		ucx_pipe_write(pipe3, hello, _strlen(hello));
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);
	ucx_task_add(task3);

	pipe1 = ucx_pipe_create(128);		/* pipe buffer, 128 bytes */
	pipe2 = ucx_pipe_create(64);		/* pipe buffer, 64 bytes */
	pipe3 = ucx_pipe_create(64);		/* pipe buffer, 64 bytes */

	// start UCX/OS, preemptive mode
	return 1;
}
