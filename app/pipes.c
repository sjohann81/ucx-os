#include <ucx.h>

struct pipe_s pipe1, pipe2, pipe3;

void task3(void)
{
	char stack[1024];
	int8_t data[128];

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		printf("Waiting data from task0... ");
		/* write pipe - write size must be less than buffer size */
		pipe_read(&pipe3, data, 127);
		printf("%s\n", data);
	}
}

void task2(void)
{
	char stack[1024];
	int8_t data[50] = "hello from task 2!";

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		pipe_write(&pipe2, data, strlen(data));
	}
}

void task1(void)
{
	char stack[1024];
	int8_t data[50] = "hello from task 1!";

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		pipe_write(&pipe1, data, strlen(data));
	}
}

void task0(void)
{
	char stack[1024];
	int8_t data1[128];	/* data buffer 1 */
	int8_t data2[50];	/* data buffer 2 */
	int8_t hello[64] = "hi!";

	ucx_task_init(stack, sizeof(stack));

	while (1) {
		/* read pipe - read size must be less than buffer size */
		pipe_read(&pipe1, data1, 127);
		printf("pipe 1: %s\n", data1);
		pipe_read(&pipe2, data2, 10);
		printf("pipe 2: %s\n", data2);
		
		pipe_write(&pipe3, hello, strlen(hello));
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);
	ucx_task_add(task3);

	pipe_init(&pipe1, 128);		/* pipe buffer, 128 bytes - powers of two */
	pipe_init(&pipe2, 64);		/* pipe buffer, 64 bytes - powers of two */
	pipe_init(&pipe3, 64);		/* pipe buffer, 64 bytes - powers of two */

	return 0;
}
