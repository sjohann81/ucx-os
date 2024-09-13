#include <ucx.h>

struct sem_s *mutex;

void task_a(void)
{
	for (;;) {
		ucx_sem_wait(mutex);
		printf("hello from task A, id %d\n", ucx_task_id());
		printf("this is still task A!\n");
		ucx_sem_signal(mutex);
	}
}

void task_b(void)
{
	for (;;) {
		ucx_sem_wait(mutex);
		printf("hello from task B, id %d\n", ucx_task_id());
		printf("this is still task B!\n");
		ucx_sem_signal(mutex);
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task_a, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task_b, DEFAULT_STACK_SIZE);

	mutex = ucx_sem_create(2, 1);
	
	return 1;
}
