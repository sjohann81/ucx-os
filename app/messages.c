#include <ucx.h>


/* application tasks */

struct mq_s *mq1, *mq2, *mq3, *mq4;

void task1(void)
{
	int val = 0;
	struct message_s msg1, msg2;
	char str[50];
	struct message_s *pmsg;
	
	while (1) {
		while (ucx_mq_items(mq1) == 0);

		printf("task 1 enters...\n");
		
		if (ucx_mq_items(mq1))
			ucx_mq_dequeue(mq1);

		pmsg = &msg1;
		pmsg->data = (void *)(size_t)val;
		ucx_mq_enqueue(mq2, pmsg);
		pmsg = &msg2;
		sprintf(str, "hello %d from t1...", val++);
		pmsg->data = (void *)&str;
		ucx_mq_enqueue(mq3, pmsg);
		
		ucx_task_yield();
	}
}

void task2(void)
{
	struct message_s msg1;
	int val = 200;
	struct message_s *msg;
	
	while (1) {
		if (ucx_mq_items(mq2) > 0) {
			printf("task 2 enters...\n");

			msg = ucx_mq_dequeue(mq2);
			printf("message %d\n", (int)(size_t)msg->data);
			msg = &msg1;
			msg->data = (void *)(size_t)val++;
			ucx_mq_enqueue(mq4, msg);
		}
		
		ucx_task_yield();
	}
}

void task3(void)
{
	struct message_s msg1;
	int val = 300;
	struct message_s *msg;
	
	while (1) {
		if (ucx_mq_items(mq3) > 0) {
			printf("task 3 enters...\n");
			
			msg = ucx_mq_dequeue(mq3);
			printf("message: %s\n", (char *)msg->data);
			msg = &msg1;
			msg->data = (void *)(size_t)val++;
			ucx_mq_enqueue(mq4, msg);
		}
	
		ucx_task_yield();
	}
}

void task4(void)
{
	struct message_s *msg1, *msg2;
	struct message_s dummy;
	
	while (1) {
		if (ucx_mq_items(mq4) > 1) {
			printf("task 4 enters...\n");

			msg1 = ucx_mq_dequeue(mq4);
			msg2 = ucx_mq_dequeue(mq4);
			printf("messages: %d %d\n", (int)(size_t)msg1->data, (int)(size_t)msg2->data);
			
			_delay_ms(100);
			
			ucx_mq_enqueue(mq1, &dummy);
		}
		
		ucx_task_yield();
	}
}

void idle(void)
{
	while (1);
}

int32_t app_main(void)
{
	ucx_task_spawn(idle, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task1, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task2, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task3, DEFAULT_STACK_SIZE);
	ucx_task_spawn(task4, DEFAULT_STACK_SIZE);
	
	mq1 = ucx_mq_create(8);
	mq2 = ucx_mq_create(8);
	mq3 = ucx_mq_create(8);
	mq4 = ucx_mq_create(8);
	
	ucx_mq_enqueue(mq1, 0);

	return 1;
}
