#include <ucx.h>
#include <ieee754.h>

/* application tasks */

enum {TYPE_STRING, TYPE_INT, TYPE_FLOAT};

struct mq_s *mq1, *mq2;

void task1(void)
{
	int val = 0;
	float fval = 0.0f;
	char str[50], int_str[20], float_str[20];
	struct message_s msg1, msg2, msg3;
	struct message_s *pmsg;
	
	while (1) {
		sprintf(str, "hello %d from t1...", val);
		pmsg = &msg1;
		pmsg->data = (void *)&str;
		ucx_mq_enqueue(mq1, pmsg);

		val++;
		pmsg = &msg2;
		itoa(val, int_str, 10);
		pmsg->data = (void *)&int_str;
		pmsg->type = TYPE_INT;
		ucx_mq_enqueue(mq2, pmsg);
		
		fval += 0.123;
		pmsg = &msg3;
		ftoa(fval, float_str, 6);
		pmsg->data = (void *)&float_str;
		pmsg->type = TYPE_FLOAT;
		ucx_mq_enqueue(mq2, pmsg);
		
		ucx_task_delay(10);
	}
}

void task2(void)
{
	struct message_s *pmsg;
	char *str;
	
	while (1) {
		if (ucx_mq_items(mq1) > 0) {
			pmsg = ucx_mq_dequeue(mq1);
			if (pmsg) {
				str = pmsg->data;
				printf("%s\n", str);
			}
		}
	}
}

void task3(void)
{
	struct message_s *pmsg;
	char *str;
	int val;
	float fval;
	char str2[50];
	
	while (1) {
		if (ucx_mq_items(mq2) > 0) {
			pmsg = ucx_mq_dequeue(mq2);
			if (pmsg) {
				str = pmsg->data;
				
				switch (pmsg->type) {
				case TYPE_INT:
					val = atoi(str);
					itoa(val, str2, 10);
					break;
				case TYPE_FLOAT:
					fval = atof(str);
					ftoa(fval, str2, 6);
					break;
				default: break;
				}
				
				printf("recv str: %s converted value: %s\n", str, str2);
			}
		}
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
	
	mq1 = ucx_mq_create(8);
	mq2 = ucx_mq_create(8);
	
	return 1;
}
