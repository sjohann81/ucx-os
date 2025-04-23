#include <ucx.h>
#include <ieee754.h>

enum {TYPE_STRING, TYPE_INT, TYPE_FLOAT};
struct mq_s *mq1, *mq2;

/* application coroutines (executed in a task context) */
struct cgroup_s *cgroup;

void *cr1(void *arg)
{
	static int val = 0;
	static float fval = 0.0f;
	static char str[50], int_str[20], float_str[20];
	static struct message_s msg1, msg2, msg3;
	static struct message_s *pmsg;
	
	sprintf(str, "hello %d from corotine 1...", val);
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
			
	return 0;
}

void *cr2(void *arg)
{
	struct message_s *pmsg;
	char *str;
	
	if (ucx_mq_items(mq1) > 0) {
		pmsg = ucx_mq_dequeue(mq1);
		if (pmsg) {
			str = pmsg->data;
			printf("%s\n", str);
		}
	}
	
	return 0;
}

void *cr3(void *arg)
{
	struct message_s *pmsg;
	char *str;
	int val;
	float fval;
	char str2[50];
	
	while (ucx_mq_items(mq2) > 0) {
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

	return 0;
}

/* only coroutines in this application */
int32_t app_main(void)
{
	uint32_t data = 0;
	
	cgroup = ucx_cr_ginit();
	
	if (!cgroup)
		printf("ucx_cr_ginit() failed!\n");
		
	ucx_cr_add(cgroup, cr1, 50);
	ucx_cr_add(cgroup, cr2, 50);
	ucx_cr_add(cgroup, cr3, 50);
	
	mq1 = ucx_mq_create(8);
	mq2 = ucx_mq_create(8);

	while (1) {
		ucx_cr_schedule(cgroup, (void *)&data);
	}
}
