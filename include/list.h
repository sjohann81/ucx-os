struct list_s {
	void *elem;
	struct list_s *next;
};

struct list_s *list_create(void);
int32_t list_destroy(struct list_s *lst);
int32_t list_add(struct list_s *lst, void *item);
void *list_peek(struct list_s *lst, int32_t pos);
int32_t list_poke(struct list_s *lst, void *item, int32_t pos);
int32_t list_count(struct list_s *lst);
int32_t list_insert(struct list_s *lst, void *item, int32_t pos);
int32_t list_remove(struct list_s *lst, int32_t pos);
