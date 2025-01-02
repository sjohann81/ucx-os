#include <ucx.h>

struct pipe_s *pipe;

void task1(void)
{
	char buf[64];
	
	while (1) {
		memset(buf, 0, sizeof(buf));
		printf("[task B] Waiting for message...\n");
		while (ucx_pipe_size(pipe) < 1);
		ucx_pipe_read(pipe, buf, ucx_pipe_size(pipe));
		printf("[task B] Message: %s\n", buf);
	}
}

void task0(void)
{
	char buf[64];
	
	while (1) {
		printf("[task A] Type a message: \n");
		gets(buf);
		ucx_pipe_write(pipe, buf, strlen(buf));
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	
	pipe = ucx_pipe_create(32);
	
	// start UCX/OS, preemptive mode
	return 1;
}
