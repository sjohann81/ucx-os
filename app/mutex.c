#include <ucx.h>

struct sem_s *mutex;

void task_a(void)
{
	for (;;) {
		ucx_wait(mutex);
		printf("hello from task A, id %d\n", ucx_task_id());
		printf("this is still task A!\n");
		ucx_signal(mutex);
	}
}

void task_b(void)
{
	for (;;) {
		ucx_wait(mutex);
		printf("hello from task B, id %d\n", ucx_task_id());
		printf("this is still task B!\n");
		ucx_signal(mutex);
	}
}

int32_t app_main(void)
{
	ucx_task_add(task_a, DEFAULT_STACK_SIZE);
	ucx_task_add(task_b, DEFAULT_STACK_SIZE);

	mutex = ucx_semcreate(1);
	
	return 1;
}
