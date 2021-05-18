#include <ucx.h>

struct pipe_s *pipe1, *pipe2, *pipe3;

void log(void)
{
	char guard[512];
	
	ucx_task_init(guard, sizeof(guard));
	
	while (1) {
		delay_ms(1000);
		hexdump((char *)(size_t)&_stack_end - 8192, 8192);
	}
}

void task3(void)
{
	int8_t data[128];
	uint16_t s;
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		memset(data, 0, sizeof(data));
		printf("Waiting data from task0... ");
		/* write pipe - write size must be less than buffer size */
		s = pipe_read(pipe3, data, 127);
		printf("pipe 3 (%d): %s\n", s, data);
	}
}

void task2(void)
{
	int8_t data[128] = "heya from task 2!";
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		pipe_write(pipe2, data, strlen(data));
	}
}

void task1(void)
{
	int8_t data[128] = "hello from task 1!";
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* write pipe - write size must be less than buffer size */
		pipe_write(pipe1, data, strlen(data));
	}
}

void task0(void)
{
	int8_t data1[128];	/* data buffer 1 */
	int8_t data2[50];	/* data buffer 2 */
	int8_t hello[64] = "hi!";
	uint16_t s;
	char guard[512];

	ucx_task_init(guard, sizeof(guard));

	while (1) {
		/* read pipe - read size must be less than buffer size */
		s = pipe_read(pipe1, data1, 127);
		printf("pipe 1 (%d): %s\n", s, data1);
		s = pipe_read(pipe2, data2, 10);
		printf("pipe 2 (%d): %s\n", s, data2);
		
		pipe_write(pipe3, hello, strlen(hello));
	}
}

int32_t app_main(void)
{
	ucx_task_add(task0);
	ucx_task_add(task1);
	ucx_task_add(task2);
	ucx_task_add(task3);
//	ucx_task_add(log);

	pipe1 = pipe_create(128);		/* pipe buffer, 128 bytes - powers of two */
	pipe2 = pipe_create(64);		/* pipe buffer, 64 bytes - powers of two */
	pipe3 = pipe_create(64);		/* pipe buffer, 64 bytes - powers of two */

	// start UCX/OS, preemptive mode
	return 1;
}
