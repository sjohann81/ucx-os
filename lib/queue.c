/* file:          queue.c
 * description:   queue data structure implementation
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

static int32_t ispowerof2(uint32_t x)
{
	return x && !(x & (x - 1));
}

static uint32_t nextpowerof2(uint32_t x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	
	return x;
}

struct queue_s *queue_create(int32_t size)
{
	struct queue_s *q;
	
	if (size < 2)
		size = 2;
	
	if (!ispowerof2(size))
		size = nextpowerof2(size);
	
	q = malloc(sizeof(struct queue_s));
	
	if (!q)
		return 0;
	
	q->size = size;
	q->mask = size - 1;
	q->pdata = malloc(q->size * sizeof(void *));
	
	if (!q->pdata) {
		free(q);
		return 0;
	}
	q->head = q->tail = 0;
	q->elem = 0;
	
	return q;
}

int32_t queue_destroy(struct queue_s *q)
{
	if (q->head == q->tail && !q->elem) {
		free(q->pdata);
		free(q);
		
		return 0;
	}
	
	return -1;
}

int32_t queue_count(struct queue_s *q)
{
	return q->elem;
}

int32_t queue_enqueue(struct queue_s *q, void *data)
{
	int32_t tail;

	tail = (q->tail + 1) & q->mask;
	if (tail == q->head)
		return -1;

	q->pdata[q->tail] = data;
	q->tail = tail;
	q->elem++;
	
	return 0;
}

void *queue_dequeue(struct queue_s *q)
{
	int32_t head;

	if (q->head == q->tail)
		return 0;

	head = q->head;
	q->head = (q->head + 1) & q->mask;
	q->elem--;

	return q->pdata[head];
}

void *queue_peek(struct queue_s *q)
{
	int32_t head;

	if (q->head == q->tail)
		return 0;

	head = q->head;

	return q->pdata[head];
}
