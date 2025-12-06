#include <ucx.h>

struct spinlock_s lock;

void task2(void)
{
	int32_t cnt = 300000;
	uint32_t secs, msecs, time;

	while (1) {
		time = ucx_uptime();
		secs = time / 1000;
		msecs = time - secs * 1000;
		ucx_lock_acquire(&lock);
		printf("[core %d, task %d %ld - sys uptime: %ld.%03lds]\n", _cpu_id(), ucx_task_id(), cnt++, secs, msecs);
		ucx_lock_release(&lock);
		ucx_task_wfi();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	while (1) {
		ucx_lock_acquire(&lock);
		printf("[core %d, task %d %ld]\n", _cpu_id(), ucx_task_id(), cnt++);
		ucx_lock_release(&lock);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		ucx_lock_acquire(&lock);
		printf("[core %d, task %d %ld]\n", _cpu_id(), ucx_task_id(), cnt++);
		ucx_lock_release(&lock);
		ucx_task_wfi();
	}
}

int app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	
	ucx_task_priority(2, TASK_LOW_PRIO);
	
	printf("task0 has id %d\n", ucx_task_idref(task0));
	printf("task1 has id %d\n", ucx_task_idref(task1));
	printf("task2 has id %d\n", ucx_task_idref(task2));
	
	ucx_lock_init(&lock);

	// start UCX/OS on core 0, preemptive mode
	return 1;
}

int app_main1(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	
	// start UCX/OS on core 1, preemptive mode
	return 1;
}

int app_main2(void)
{
	while (1) {
		ucx_lock_acquire(&lock);
		printf("hello from core %d\n", _cpu_id());
		ucx_lock_release(&lock);
		_delay_ms(50);
	}	
}
