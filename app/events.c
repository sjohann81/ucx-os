#include <ucx.h>


/* callbacks used on events */

void *callback1(void *arg)
{
	printf("callback 1 (arg: %d)\n", (int)arg);
	
	return 0;
}

void *callback2(void *arg)
{
	printf("callback 2 (arg: %d)\n", (int)arg);
	
	return 0;
}

void *callback3(void *arg)
{
	printf("callback 3\n");
	
	return 0;
}


/* application tasks */

struct eq_s *eq1, *eq2;

void idle(void)
{
	for (;;);
}

void task1(void)
{
	struct event_s *event;
	
	for (;;) {
		ucx_task_delay(10);
		event = malloc(sizeof(struct event_s));
		event->callback = callback1;
		ucx_event_post(eq1, event);
	}
}

void task2(void)
{
	struct event_s *event1;
	struct event_s *event2;
	int i = 0;
	
	for (;;) {
		ucx_task_delay(10);
		event1 = malloc(sizeof(struct event_s));
		event1->callback = callback2;
		ucx_event_post(eq1, event1);
		event2 = malloc(sizeof(struct event_s));
		event2->callback = 0;
		event2->data = (void *)i++;
		ucx_event_post(eq1, event2);
	}
}

void task3(void)
{
	struct event_s *eptr;
	int i = 12345;
	int items;
	
	for (;;) {
		items = ucx_event_poll(eq1);
		if (items) {
			printf("items: %d\n", items);
			eptr = ucx_event_get(eq1);
			if (eptr->callback)
				ucx_event_dispatch(eptr, (void *)i--);
			else
				printf("data: %d\n", (int)eptr->data);
			free(eptr);
		}
	}
}

void task4(void)
{
	struct event_s event;
	
	for (;;) {
		ucx_task_delay(50);
		event.callback = callback3;
		ucx_event_post(eq2, &event);
	}
}

void task5(void)
{
	struct event_s *eptr;
	
	for (;;) {
		if (ucx_event_poll(eq2)) {
			eptr = ucx_event_get(eq2);
			if (eptr->callback)
				ucx_event_dispatch(eptr, 0);
		}
	}
}

int32_t app_main(void)
{
	ucx_task_add(idle, DEFAULT_STACK_SIZE);
	ucx_task_add(task1, DEFAULT_STACK_SIZE);
	ucx_task_add(task2, DEFAULT_STACK_SIZE);
	ucx_task_add(task3, DEFAULT_STACK_SIZE);
	ucx_task_add(task4, DEFAULT_STACK_SIZE);
	ucx_task_add(task5, DEFAULT_STACK_SIZE);
	
	eq1 = ucx_eq_create(16);
	eq2 = ucx_eq_create(4);
	
	return 1;
}
