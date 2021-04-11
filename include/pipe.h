struct pipe_s {
	int8_t *data;
	volatile uint32_t mask;				/* size must be a power of 2 */
	volatile int32_t head, tail, size;
};

struct pipe_s *pipe_create(uint16_t size);
int32_t pipe_destroy(struct pipe_s *pipe);
void pipe_flush(struct pipe_s *pipe);
int32_t pipe_size(struct pipe_s *pipe);
int8_t pipe_get(struct pipe_s *pipe);
int32_t pipe_put(struct pipe_s *pipe, int8_t data);
int32_t pipe_read(struct pipe_s *pipe, int8_t *data, uint16_t size);
int32_t pipe_write(struct pipe_s *pipe, int8_t *data, uint16_t size);
