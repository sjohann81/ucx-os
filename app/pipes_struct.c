#include <ucx.h>

struct pipe_s *pipe1;

struct data1_s {
	char v[20];
	int32_t a;
	int16_t b;
};

void task1(void)
{
	char data1[64];
	int32_t i = 0;
	struct data1_s *ptr = (struct data1_s *)&data1;

	ptr->a = 12345;
	ptr->b = -555;

	while (1) {
		sprintf(ptr->v, "hello %ld", i++);
		ptr->a++;
		ptr->b++;

		ucx_pipe_write(pipe1, data1, sizeof(struct data1_s));
		
		_delay_ms(500);
	}
}

void task0(void)
{
	char data[64];
	uint16_t s;
	struct data1_s *ptr = (struct data1_s *)&data;

	while (1) {
		while (ucx_pipe_size(pipe1) < sizeof(struct data1_s));
		s = ucx_pipe_read(pipe1, data, ucx_pipe_size(pipe1));
		printf("pipe (%d): %s %ld %d\n", s, ptr->v, ptr->a, ptr->b);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);

	pipe1 = ucx_pipe_create(64);		/* pipe buffer, 64 bytes */
	
	if (!pipe1)
		printf("Fail!\n");

	// start UCX/OS, preemptive mode
	return 1;
}
