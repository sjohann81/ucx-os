#include <ucx.h>

/* application tasks */
void task4(void)
{
	while (1) {
		printf("Now I run too!\n");
		ucx_task_wfi();
	}
}

void task3(void)
{
	while (1) {
		printf("Now I run\n");
		ucx_task_wfi();
	}
}

void task2(void)
{
	int32_t cnt = 300000;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task1(void)
{
	int32_t cnt = 200000;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}

void task0(void)
{
	int32_t cnt = 100000;

	while (1) {
		printf("[task %d %ld]\n", ucx_task_id(), cnt++);
		ucx_task_wfi();
	}
}


/* Our custom scheduler
 * 
 * This scheduler is used as an example, as it is not a RT scheduling
 * policy. The algorithm is based on credits assigned for each task.
 * Each time a task is scheduled, it loses a credit. When all credits
 * expire, the credits are renewed and a new task is taken from the
 * list of "RT" tasks. When all "RT" tasks have run, let the kernel
 * schedule a non RT task.
 */

struct our_priority_s {
	unsigned credits;
	unsigned remaining;
};

int32_t our_sched(void)
{
	static struct node_s *task_node = 0;
	struct our_priority_s *priority;
	struct tcb_s *task = kcb->task_current->data;

	/* if the current preempted task is not blocked or suspended, it is ready */
	if (task->state == TASK_RUNNING)
		task->state = TASK_READY;
	
	/* first run of this scheduler after the default scheduler */
	if (!task_node) {
		task_node = kcb->tasks->head;
		
		do {
			task_node = list_next(task_node);
			task = task_node->data;
		} while (task->state != TASK_READY || !task->rt_prio);
	} else {
		/* get current RT task priority */
		task = task_node->data;
		priority = task->rt_prio;

		/* if the task has no credit, reset credits and choose another one */
		if (!(--priority->remaining)) {
			priority->remaining = priority->credits;

			/* get the next RT task */
			do {
				/* we scheduled all RT tasks */
				/* let the kernel schedule a non RT task */
				if (task_node == kcb->tasks->tail) {
					task_node = 0;
					
					return -1;
				}
				
				task_node = list_next(task_node);
				task = task_node->data;
			} while (task->state != TASK_READY || !task->rt_prio);
		}
	}

	/* put the scheduled task in the running state and return its id */
	kcb->task_current = task_node;
	task->state = TASK_RUNNING;
	
	return task->id;
}

int32_t app_main(void)
{
	/* Define RT task priorities (3, 4 and 5 credits) */
	struct our_priority_s priorities[3] = {
		{.credits = 3, .remaining = 3},
		{.credits = 4, .remaining = 4},
		{.credits = 5, .remaining = 5}
	};
	
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task3, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task4, DEFAULT_STACK_SIZE);

	/* Setup our custom scheduler and set RT priorities to three
	 * tasks of the set */ 
	kcb->rt_sched = our_sched;
	ucx_task_rt_priority(0, &priorities[0]);
	ucx_task_rt_priority(1, &priorities[1]);
	ucx_task_rt_priority(2, &priorities[2]);
	
	// start UCX/OS, preemptive mode
	return 1;
}
