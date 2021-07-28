/* file:          pipe.c
 * description:   pipe implementation
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

static int32_t ispowerof2(uint32_t x){
	return x && !(x & (x - 1));
}

struct pipe_s *ucx_pipe_create(uint16_t size)
{
	struct pipe_s *pipe;
	
	if (!ispowerof2(size))
		return 0;
		
	pipe = (struct pipe_s *)_malloc(sizeof(struct pipe_s));
	
	if (!pipe)
		return 0;
	
	pipe->mask = size - 1;
	pipe->data = (char *)_malloc(size);
	if (!pipe->data) {
		_free(pipe);
		return 0;
	}
	pipe->head = 0;
	pipe->tail = 0;
	pipe->size = 0;
	
	return pipe;
}

int32_t ucx_pipe_destroy(struct pipe_s *pipe)
{
	if (!pipe->data)
		return -1;
	
	pipe->mask = 0;
	_free(pipe->data);
	_free(pipe);
	
	return 0;
}

void ucx_pipe_flush(struct pipe_s *pipe)
{
	pipe->head = 0;
	pipe->tail = 0;
	pipe->size = 0;
}

int32_t ucx_pipe_size(struct pipe_s *pipe)
{
	return pipe->size;
}

int32_t ucx_pipe_get(struct pipe_s *pipe)
{
	int32_t head;

	if (pipe->head == pipe->tail)
		return -1;

	head = pipe->head;
	pipe->head = (pipe->head + 1) & pipe->mask;
	pipe->size--;

	return pipe->data[head];
}

int32_t ucx_pipe_put(struct pipe_s *pipe, char data)
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

/* this routine is blocking and must be called inside a task. */
int32_t ucx_pipe_read(struct pipe_s *pipe, char *data, uint16_t size)
{
	uint16_t i = 0;
	int32_t byte;
	
	while (i < size) {
		ucx_enter_critical();
		byte = ucx_pipe_get(pipe);
		ucx_leave_critical();
		if (byte == -1) {
			_delay_ms(1);
			continue;
		}
		data[i] = byte;
		i++;
	}
	
	return i;
	
}

/* this routine is blocking and must be called inside a task. */
int32_t ucx_pipe_write(struct pipe_s *pipe, char *data, uint16_t size)
{
	uint16_t i = 0;
	int32_t res;
	
	while (i < size) {
		ucx_enter_critical();
		res = ucx_pipe_put(pipe, data[i]);
		ucx_leave_critical();
		if (res == -1) {
			_delay_ms(1);
			continue;
		}
		i++;
	}

	return i;
}
