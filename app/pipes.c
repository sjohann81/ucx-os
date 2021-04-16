#include <ucx.h>

struct pipe_s *pipe1, *pipe2, *pipe3;

void log(void)
{
	char guard[512];
	int32_t i;
	
	ucx_task_init(guard, sizeof(guard));
	
	while (1) {
	}
}

void task3(void)
{
	char guard[512];
	int8_t data[128];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		printf("Waiting data from task0... ");
		/* write pipe - write size must be less than buffer size */
		pipe_read(pipe3, data, 127);
		printf("%s\n", data);
	}
}

void task2(void)
{
	char guard[512];
	int8_t data[128] = "hello from task 2!";

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		pipe_write(pipe2, data, strlen(data) + 1);
	}
}

void task1(void)
{
	char guard[512];
	int8_t data[128] = "hello from task 1!";

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		pipe_write(pipe1, data, strlen(data) + 1);
	}
}

void task0(void)
{
	char guard[512];
	int8_t data1[128];	/* data buffer 1 */
	int8_t data2[50];	/* data buffer 2 */
	int8_t hello[64] = "hi!";

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* read pipe - read size must be less than buffer size */
		pipe_read(pipe1, data1, 127);
		printf("pipe 1: %s\n", data1);
		pipe_read(pipe2, data2, 10);
		printf("pipe 2: %s\n", data2);
		
		pipe_write(pipe3, hello, strlen(hello) + 1);
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);
	ucx_task_add(task3);
	ucx_task_add(log);

	pipe1 = pipe_create(128);		/* pipe buffer, 128 bytes - powers of two */
	pipe2 = pipe_create(64);		/* pipe buffer, 64 bytes - powers of two */
	pipe3 = pipe_create(64);		/* pipe buffer, 64 bytes - powers of two */

	// start UCX/OS, preemptive mode
	return 1;
}
