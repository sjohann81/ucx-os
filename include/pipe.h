struct pipe_s {
	int8_t *data;
	uint32_t mask;				/* size must be a power of 2 */
	int32_t head, tail, size;
};

int32_t pipe_init(volatile struct pipe_s *pipe, uint16_t size);
int32_t pipe_finish(volatile struct pipe_s *pipe);
void pipe_flush(volatile struct pipe_s *pipe);
int32_t pipe_size(volatile struct pipe_s *pipe);
int8_t pipe_get(volatile struct pipe_s *pipe);
int32_t pipe_put(volatile struct pipe_s *pipe, int8_t data);
int32_t pipe_read(volatile struct pipe_s *pipe, int8_t *data, uint16_t size);
int32_t pipe_write(volatile struct pipe_s *pipe, int8_t *data, uint16_t size);
