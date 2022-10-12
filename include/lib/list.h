struct list_s {
	void *elem;
	struct list_s *next;
};

struct list_s *ucx_list_create(void);
int32_t ucx_list_destroy(struct list_s *lst);
int32_t ucx_list_add(struct list_s *lst, void *item);
void *ucx_list_peek(struct list_s *lst, int32_t pos);
int32_t ucx_list_poke(struct list_s *lst, void *item, int32_t pos);
int32_t ucx_list_count(struct list_s *lst);
int32_t ucx_list_insert(struct list_s *lst, void *item, int32_t pos);
int32_t ucx_list_remove(struct list_s *lst, int32_t pos);
