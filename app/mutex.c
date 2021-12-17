#include <ucx.h>

struct sem_s *mutex;

void task_a(void)
{
	ucx_task_init();

	for (;;) {
		ucx_wait(mutex);
		_printf("hello from task A, id %d\n", ucx_task_id());
		_printf("this is still task A!\n");
		ucx_signal(mutex);
	}
}

void task_b(void)
{
	ucx_task_init();

	for (;;) {
		ucx_wait(mutex);
		_printf("hello from task B, id %d\n", ucx_task_id());
		_printf("this is still task B!\n");
		ucx_signal(mutex);
	}
}

int32_t app_main(void)
{
	ucx_task_add(task_a, DEFAULT_GUARD_SIZE);
	ucx_task_add(task_b, DEFAULT_GUARD_SIZE);

	mutex = ucx_seminit(1);
	
	return 1;
}
