#include <ucx.h>

/* application coroutines (executed in a task context) */
struct cgroup_s *cgroup;
struct spinlock_s lock;

void *cr1(void *arg)
{
	static uint32_t count = 0;
	
	printf("hello from corotine 1 [%d]\n", count++);
	
	return 0;
}

void *cr2(void *arg)
{
	static uint32_t count = 0;
	
	printf("hello from corotine 2 [%d]\n", count++);
	
	return 0;
}


/* application tasks */
void task1(void)
{
	int32_t cnt = 200000;
	uint32_t secs, msecs, time;

	while (1) {
		time = ucx_uptime();
		secs = time / 1000;
		msecs = time - secs * 1000;
		ucx_lock_acquire(&lock);
		printf("[core %d, task %d %ld - sys uptime: %ld.%03lds]\n", _cpu_id(), ucx_task_id(), cnt++, secs, msecs);
		ucx_lock_release(&lock);
		ucx_task_yield();
	}
}

void task0(void)
{
	int32_t cnt = 200000;
	uint32_t secs, msecs, time;

	while (1) {
		time = ucx_uptime();
		secs = time / 1000;
		msecs = time - secs * 1000;
		ucx_lock_acquire(&lock);
		printf("[core %d, task %d %ld - sys uptime: %ld.%03lds]\n", _cpu_id(), ucx_task_id(), cnt++, secs, msecs);
		ucx_lock_release(&lock);
		ucx_task_yield();
	}
}


int app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_priority(1, TASK_LOW_PRIO);
	
	ucx_lock_init(&lock);
	
	// start UCX/OS, preemptive mode
	return 1;
}

int app_main1(void)
{
	int32_t cnt = 100000;

	cgroup = ucx_cr_ginit();
	
	if (!cgroup) {
		printf("ucx_cr_ginit() failed!\n");
		return -1;
	}
		
	ucx_cr_add(cgroup, cr1, 5);
	ucx_cr_add(cgroup, cr2, 15);

	while (1) {
		ucx_lock_acquire(&lock);
		printf("[core %d %ld]\n", _cpu_id(), cnt++);
		ucx_cr_schedule(cgroup, (void *)0);
		ucx_lock_release(&lock);
		_delay_ms(20);
	}
}
