/* file:          malloc.c
 * description:   memory allocator and heap management
 * 
 * memory allocator using first-fit
 * 
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct mem_block_s *first_free;
struct mem_block_s *last_free;

void ucx_free(void *ptr)
{
	struct mem_block_s *p;
	
	p = ((struct mem_block_s *)ptr) - 1;
	p->size &= ~1L;
	last_free = first_free;
}

void *ucx_malloc(uint32_t size)
{
	struct mem_block_s *p, *q, *r, n;
	
	size = align4(size);
	p = last_free;
	q = p;

	while (p->next) {
		while (p->size & 1) {
			p = p->next;
			q = p;
		}
		while (!(p->size & 1) && p->next)
			p = p->next;
		if (p) {
			q->size = (size_t)p - (size_t)q - sizeof(struct mem_block_s);
			q->next = p;
		}
		if (q->size >= size + sizeof(struct mem_block_s)) {
			p = q;
			break;
		}
	}

	if (p->next == 0)
		return 0;
	
	last_free = p;
	r = p->next;
	p->next = (struct mem_block_s *)((size_t)p + size + sizeof(struct mem_block_s));
	p->size = size | 1;
	n.next = r;
	n.size = (p->size & ~1L) - size - sizeof(struct mem_block_s);
	*p->next = n;
	
	return (void *)(p + 1);
}

void ucx_heap_init(size_t *zone, uint32_t len)
{
	void *heap = zone;
	struct mem_block_s *p = (struct mem_block_s *)heap;
	struct mem_block_s *q = (struct mem_block_s *)((size_t)(struct mem_block_s *)heap + len - (sizeof(struct mem_block_s)));
	
	len = align4(len);
	p->next = q;
	p->size = len - sizeof(struct mem_block_s) - sizeof(struct mem_block_s);
	q->next = 0;
	q->size = 0;
	first_free = (struct mem_block_s *)heap;
	last_free = (struct mem_block_s *)heap;
}

void *ucx_calloc(uint32_t size, uint32_t type_size)
{
	void *buf;
	
	buf = (void *)malloc((size * type_size));
	if (buf)
		memset(buf, 0, (size * type_size));

	return (void *)buf;
}

void *ucx_realloc(void *ptr, uint32_t size)
{
	void *buf;

	if ((int32_t)size < 0)
		return NULL;
		
	if (ptr == NULL)
		return (void *)malloc(size);

	buf = (void *)malloc(size);
	
	if (buf){
		memcpy(buf, ptr, size);
		free(ptr);
	}

	return (void *)buf;
}
