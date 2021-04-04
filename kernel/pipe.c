/* file:          pipe.c
 * description:   pipe implementation
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

static int32_t ispowerof2(uint32_t x){
	return x && !(x & (x - 1));
}

int32_t pipe_init(volatile struct pipe_s *pipe, uint16_t size)
{
	if (!ispowerof2(size)) return -1;
	
	pipe->mask = size - 1;
	pipe->data = (int8_t *)malloc(size);
	if (!pipe->data) return -1;
	
	return 0;
}

int32_t pipe_finish(volatile struct pipe_s *pipe)
{
	if (!pipe->data) return -1;
	
	pipe->mask = 0;
	free(pipe->data);
	
	return 0;
}

void pipe_flush(volatile struct pipe_s *pipe)
{
	pipe->head = 0;
	pipe->tail = 0;
	pipe->size = 0;
}

int32_t pipe_size(volatile struct pipe_s *pipe)
{
	return pipe->size;
}

int8_t pipe_get(volatile struct pipe_s *pipe)
{
	int32_t head;

	if (pipe->head == pipe->tail)
		return 0;

	head = pipe->head;
	pipe->head = (pipe->head + 1) & pipe->mask;
	pipe->size--;

	return pipe->data[head];
}

int32_t pipe_put(volatile struct pipe_s *pipe, int8_t data)
{
	int32_t tail;

	tail = (pipe->tail + 1) & pipe->mask;
	if (tail == pipe->head)
		return -1;

	pipe->data[pipe->tail] = data;
	pipe->tail = tail;
	pipe->size++;

	return 0;
}


int32_t pipe_read(volatile struct pipe_s *pipe, int8_t *data, uint16_t size)
{
	uint16_t i;
	
	for (i = 0; i < size; i++) {
		data[i] = pipe_get(pipe);
		if (data[i] == '\0') break;
	}
	
	return i;
	
}

int32_t pipe_write(volatile struct pipe_s *pipe, int8_t *data, uint16_t size)
{
	uint16_t i;
	
	for (i = 0; i < size; i++)
		if (pipe_put(pipe, data[i]) == -1) break;
	
	return i;
}
