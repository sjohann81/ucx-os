#include <ucx.h>

#define N 10

struct sem_s *empty, *full, *mutex;
int32_t in = 0, out = 0, buffer[N];

void producer(void)
{
	int32_t item;

	for (;;) {
		item = random();
		ucx_wait(empty);
		ucx_wait(mutex);
		buffer[in] = item;
		printf("\nproducer %d putting at %ld (%ld)", ucx_task_id(), in, item);
		in = (in + 1) % N;
		ucx_signal(mutex);
		ucx_signal(full);
	}
}

void consumer(void)
{
	int32_t item;

	for (;;) {
		ucx_wait(full);
		ucx_wait(mutex);
		item = buffer[out];
		printf("\nconsumer %d getting from %ld (%ld)", ucx_task_id(), out, item);
		out = (out + 1) % N;
		ucx_signal(mutex);
		ucx_signal(empty);
	}
}

int32_t app_main(void)
{
	ucx_task_add(producer, DEFAULT_STACK_SIZE);
	ucx_task_add(consumer, DEFAULT_STACK_SIZE);
	ucx_task_add(consumer, DEFAULT_STACK_SIZE);

	empty = ucx_semcreate(N);
	full = ucx_semcreate(0);
	mutex = ucx_semcreate(1);
	
	return 1;
}
