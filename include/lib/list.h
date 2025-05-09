struct node_s {
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
struct node_s *list_next(struct node_s *node);
struct node_s *list_cnext(struct list_s *list, struct node_s *node);
void *list_move(struct list_s *list_dst, struct list_s *list_src, struct node_s *node);
struct node_s *list_rotate(struct list_s *list);
struct node_s *list_insert(struct list_s *list, struct node_s *prevnode, void *val);
void *list_remove(struct list_s *list, struct node_s *node);
struct node_s *list_index(struct list_s *list, int idx);
struct node_s *list_foreach(struct list_s *list, struct node_s *(*iter_fn)(struct node_s *, void *), void *arg);

struct dnode_s {
	struct dnode_s *prev;
	struct dnode_s *next;
	void *data;
};

struct dlist_s {
	struct dnode_s *head;
	struct dnode_s *tail;
	int length;
};

struct dlist_s *dlist_create();
int dlist_destroy(struct dlist_s *list);
struct dnode_s *dlist_push(struct dlist_s *list, void *val);
struct dnode_s *dlist_pushback(struct dlist_s *list, void *val);
void *dlist_pop(struct dlist_s *list);
void *dlist_popback(struct dlist_s *list);
struct dnode_s *dlist_insert(struct dlist_s *list, struct dnode_s *prevnode, void *val);
void *dlist_remove(struct dlist_s *list, struct dnode_s *node);
struct dnode_s *dlist_index(struct dlist_s *list, int idx);
struct dnode_s *dlist_foreach(struct dlist_s *list, struct dnode_s *(*iter_fn)(struct dnode_s *, void *), void *arg);
