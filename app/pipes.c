#include <ucx.h>

struct pipe_s *pipe1, *pipe2, *pipe3;

char data1[128] = "hello from task 1!";
char data2[128] = "heya from task 2!";
char hello[64] = "hi!";

void task3(void)
{
	char data[128];
	uint16_t s;

	while (1) {
		memset(data, 0, sizeof(data));
		printf("Waiting data from task0... ");
		/* read pipe - read size must be less than buffer size */
		s = ucx_pipe_read(pipe3, data, 127);
		printf("pipe 3 (%d): %s\n", s, data);
	}
}

void task2(void)
{
	while (1) {
		/* write pipe - write size must be less than buffer size */
		ucx_pipe_write(pipe2, data2, strlen(data2));
	}
}

void task1(void)
{
	while (1) {
		/* write pipe - write size must be less than buffer size */
		ucx_pipe_write(pipe1, data1, strlen(data1));
	}
}

void task0(void)
{
	char data1[128];	/* data buffer 1 */
	char data2[50];		/* data buffer 2 */
	uint16_t s;

	while (1) {
		/* read pipe - read size must be less than buffer size */
		memset(data1, 0, sizeof(data1));
		s = ucx_pipe_read(pipe1, data1, 127);
		printf("pipe 1 (%d): %s\n", s, data1);
		memset(data2, 0, sizeof(data2));
		s = ucx_pipe_read(pipe2, data2, 10);
		printf("pipe 2 (%d): %s\n", s, data2);
		
		ucx_pipe_write(pipe3, hello, strlen(hello));
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task3, DEFAULT_STACK_SIZE);

	pipe1 = ucx_pipe_create(128);		/* pipe buffer, 128 bytes (allocated on the heap) */
	pipe2 = ucx_pipe_create(64);		/* pipe buffer, 64 bytes */
	pipe3 = ucx_pipe_create(64);		/* pipe buffer, 64 bytes */

	// start UCX/OS, preemptive mode
	return 1;
}
