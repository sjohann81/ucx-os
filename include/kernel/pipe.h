struct pipe_s {
	char *data;
	uint32_t mask;				/* size must be a power of 2 */
	int32_t head, tail, size;
};

struct pipe_s *ucx_pipe_create(uint16_t size);
int32_t ucx_pipe_destroy(struct pipe_s *pipe);
void ucx_pipe_flush(struct pipe_s *pipe);
int32_t ucx_pipe_size(struct pipe_s *pipe);
int32_t ucx_pipe_read(struct pipe_s *pipe, char *data, uint16_t size);
int32_t ucx_pipe_write(struct pipe_s *pipe, char *data, uint16_t size);
int32_t ucx_pipe_nbread(struct pipe_s *pipe, char *data, uint16_t size);
int32_t ucx_pipe_nbwrite(struct pipe_s *pipe, char *data, uint16_t size);
