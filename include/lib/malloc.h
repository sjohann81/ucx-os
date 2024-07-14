#define align4(x) ((((x) + 3) >> 2) << 2)

struct mem_block_s {
	struct mem_block_s *next;		/* pointer to the next block */
	size_t size;				/* aligned block size. the LSB is used to define if the block is used */
};

void ucx_free(void *ptr);
void *ucx_malloc(uint32_t size);
void ucx_heap_init(size_t *zone, uint32_t len);
void *ucx_calloc(uint32_t size, uint32_t type_size);
void *ucx_realloc(void *ptr, uint32_t size);

