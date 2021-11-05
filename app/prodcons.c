#include <ucx.h>

#define N 10

struct sem_s empty, full, mutex;
int32_t in = 0, out = 0, buffer[N];

void producer(void)
{
	int32_t item;
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	for (;;) {
		item = _random();
		ucx_wait(&empty);
		ucx_wait(&mutex);
		buffer[in] = item;
		_printf("\nproducer %d putting at %ld (%ld)", ucx_task_id(), in, item);
		in = (in + 1) % N;
		ucx_signal(&mutex);
		ucx_signal(&full);
	}
}

void consumer(void)
{
	int32_t item;
	char guard[256];

	ucx_task_init(guard, sizeof(guard));

	for (;;) {
		ucx_wait(&full);
		ucx_wait(&mutex);
		item = buffer[out];
		_printf("\nconsumer %d getting from %ld (%ld)", ucx_task_id(), out, item);
		out = (out + 1) % N;
		ucx_signal(&mutex);
		ucx_signal(&empty);
	}
}

int32_t app_main(void)
{
	ucx_task_add(producer);
	ucx_task_add(consumer);
	ucx_task_add(consumer);

	ucx_seminit(&empty, N);
	ucx_seminit(&full, 0);
	ucx_seminit(&mutex, 1);
	
	return 1;
}
