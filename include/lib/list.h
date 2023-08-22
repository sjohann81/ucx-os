struct node_s {
	struct node_s *prev;
	struct node_s *next;
	void *data;
};

struct list_s {
	struct node_s *head;
	struct node_s *tail;
	int length;
};

struct list_s *list_create();
int list_destroy(struct list_s *list);
struct node_s *list_push(struct list_s *list, void *val);
struct node_s *list_pushback(struct list_s *list, void *val);
void *list_pop(struct list_s *list);
void *list_popback(struct list_s *list);
struct node_s *list_insert(struct list_s *list, struct node_s *prevnode, void *val);
struct node_s *list_remove(struct list_s *list, struct node_s *node);
struct node_s *list_index(struct list_s *list, int idx);
struct node_s *list_foreach(struct list_s *list, struct node_s *(*iter_fn)(struct node_s *, void *), void *arg);
