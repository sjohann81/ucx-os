/* file:          list.c
 * description:   singly and doubly linked list data structure implementation
 * date:          08/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>


/* singly linked list */

struct list_s *list_create()
{
	struct list_s *list;
	struct node_s *head, *tail;
	
	list = malloc(sizeof(struct list_s));
	
	if (!list)
		return 0;
	
	head = malloc(sizeof(struct node_s));
	
	if (!head) {
		free(list);
		
		return 0;
	}
		
	tail = malloc(sizeof(struct node_s));
	
	if (!tail) {
		free(list);
		free(head);
		
		return 0;
	}
	
	head->next = tail;
	head->data = (void *)0xdead;
	tail->next = 0;
	tail->data = 0;
	
	list->head = head;
	list->tail = tail;
	list->length = 0;
		
	return list;
}

int list_destroy(struct list_s *list)
{
	if (list->head->next != list->tail)
		return -1;
		
	free(list->tail);
	free(list->head);
	free(list);
	
	return 0;
}

struct node_s *list_push(struct list_s *list, void *val)
{
	struct node_s *node;
	
	node = malloc(sizeof(struct node_s));
	
	if (!node)
		return 0;
	
	node->data = val;
	node->next = list->head->next;	
	list->head->next = node;
	list->length++;
	
	return node;
}

struct node_s *list_pushback(struct list_s *list, void *val)
{
	struct node_s *node;
	struct node_s *last;
	
	node = malloc(sizeof(struct node_s));
	
	if (!node)
		return 0;

	node->data = 0;
	node->next = 0;
	last = list->tail;
	list->tail->next = node;
	list->tail->data = val;
	list->tail = node;
	list->length++;
	
	return last;
}

void *list_pop(struct list_s *list)
{
	struct node_s *node;
	void *val;

	if (!list->head->next->next)
		return 0;
	
	node = list->head->next;
	val = node->data;
	
	list->head->next = node->next;
	list->length--;
	
	free(node);
	
	return val;
}

void *list_popback(struct list_s *list)
{
	struct node_s *node;
	struct node_s *last;
	void *val;

	if (!list->head->next->next)
		return 0;

	last = list->head;
	while (last->next->next->next)
		last = last->next;
	
	val = last->next->data;
	node = last->next;
	
	last->next = list->tail;
	list->length--;
	
	free(node);
	
	return val;
}

struct node_s *list_next(struct node_s *node)
{
	return node->next;
}

struct node_s *list_cnext(struct list_s *list, struct node_s *node)
{
	if (node->next->next == 0)
		return list->head->next;
	else
		return node->next;
}

void *list_move(struct list_s *list_dst, struct list_s *list_src, struct node_s *node)
{
	struct node_s *last;
	void *val;
	
	if (node->next == 0 || node == 0)
		return 0;
	
	val = node->data;
	
	last = list_src->head;
	while (last->next != node)
		last = last->next;
	
	last->next = node->next;
	list_src->length--;
	
	node->next = 0;
	last = list_dst->tail;
	list_dst->tail->next = node;
	list_dst->tail->data = val;
	list_dst->tail = node;
	node->data = 0;
	list_dst->length++;
	
	return val;
}

struct node_s *list_rotate(struct list_s *list)
{
	struct node_s *node;
	struct node_s *last;

	if (!list->head->next->next)
		return 0;
	
	node = list->head->next;
	list->head->next = node->next;
	last = list->tail;
	list->tail->next = node;
	list->tail->data = node->data;
	list->tail = node;
	node->data = 0;
	node->next = 0;
	
	return last;
}

struct node_s *list_insert(struct list_s *list, struct node_s *prevnode, void *val)
{
	struct node_s *node;
	
	node = malloc(sizeof(struct node_s));
	
	if (!node)
		return 0;
	
	node->data = val;
	node->next = prevnode->next;
	
	if (prevnode->next) {
		prevnode->next = node;
	} else {
		free(node);
		
		return 0;
	}
		
	list->length++;
	
	return node;	
}

void *list_remove(struct list_s *list, struct node_s *node)
{
	struct node_s *last;
	void *val;
	
	if (node->next == 0 || node == 0)
		return 0;
	
	val = node->data;
	
	last = list->head;
	while (last->next != node)
		last = last->next;
	
	last->next = node->next;
	list->length--;
	free(node);
	
	return val;
}

struct node_s *list_index(struct list_s *list, int idx)
{
	struct node_s *node;
	
	if (idx >= 0) {
		node = list->head->next;
		for (int i = 0; node && i < idx; i++)
			node = node->next;
	} else {
		return 0;
	}
	
	return node;
}

struct node_s *list_foreach(struct list_s *list, struct node_s *(*iter_fn)(struct node_s *, void *), void *arg)
{
	struct node_s *node, *val;

	node = list->head->next;
	
	while (node->next) {
		val = iter_fn(node, arg);
		
		if (val)
			return val;
			
		node = node->next;
	}
	
	return 0;
}


/* doubly linked list */

struct dlist_s *dlist_create()
{
	struct dlist_s *list;
	struct dnode_s *head, *tail;
	
	list = malloc(sizeof(struct dlist_s));
	
	if (!list)
		return 0;
	
	head = malloc(sizeof(struct dnode_s));
	
	if (!head) {
		free(list);
		
		return 0;
	}
		
	tail = malloc(sizeof(struct dnode_s));
	
	if (!tail) {
		free(list);
		free(head);
		
		return 0;
	}
	
	head->prev = 0;
	head->next = tail;
	head->data = (void *)0xdead;;
	tail->prev = head;
	tail->next = 0;
	tail->data = (void *)0xbeef;
	
	list->head = head;
	list->tail = tail;
	list->length = 0;
		
	return list;
}

int dlist_destroy(struct dlist_s *list)
{
	if (list->head->next != list->tail)
		return -1;
		
	free(list->tail);
	free(list->head);
	free(list);
	
	return 0;
}

struct dnode_s *dlist_push(struct dlist_s *list, void *val)
{
	struct dnode_s *node;
	
	node = malloc(sizeof(struct dnode_s));
	
	if (!node)
		return 0;
	
	node->data = val;
	node->prev = list->head;
	node->next = list->head->next;	
	list->head->next->prev = node;
	list->head->next = node;
	list->length++;
	
	return node;
}

struct dnode_s *dlist_pushback(struct dlist_s *list, void *val)
{
	struct dnode_s *node;
	
	node = malloc(sizeof(struct dnode_s));
	
	if (!node)
		return 0;
	
	node->data = val;
	node->prev = list->tail->prev;
	node->next = list->tail;	
	list->tail->prev->next = node;
	list->tail->prev = node;
	list->length++;
	
	return node;
}

void *dlist_pop(struct dlist_s *list)
{
	struct dnode_s *node;
	void *val;

	if (!list->head->next->next)
		return 0;
	
	node = list->head->next;
	val = node->data;
	
	node->next->prev = list->head;
	list->head->next = node->next;
	list->length--;
	
	free(node);
	
	return val;
}

void *dlist_popback(struct dlist_s *list)
{
	struct dnode_s *node;
	void *val;

	if (!list->head->next->next)
		return 0;
	
	node = list->tail->prev;
	val = node->data;
	
	node->prev->next = list->tail;
	list->tail->prev = node->prev;
	list->length--;
	
	free(node);
	
	return val;
}

struct dnode_s *dlist_insert(struct dlist_s *list, struct dnode_s *prevnode, void *val)
{
	struct dnode_s *node;
	
	node = malloc(sizeof(struct dnode_s));
	
	if (!node)
		return 0;
	
	node->data = val;
	node->prev = prevnode;
	node->next = prevnode->next;
	
	if (prevnode->next) {
		prevnode->next->prev = node;
		prevnode->next = node;
	} else {
		free(node);
		
		return 0;
	}
		
	list->length++;
	
	return node;	
}

void *dlist_remove(struct dlist_s *list, struct dnode_s *node)
{
	void *val;
	
	if (node->prev == 0 || node->next == 0 || node == 0)
		return 0;
	
	val = node->data;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list->length--;
	free(node);
	
	return val;
}

struct dnode_s *dlist_index(struct dlist_s *list, int idx)
{
	struct dnode_s *node;
	
	if (idx >= 0) {
		node = list->head->next;
		for (int i = 0; node && i < idx; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (int i = 0; node && i > idx; i--)
			node = node->prev;
	}
	
	return node;
}

struct dnode_s *dlist_foreach(struct dlist_s *list, struct dnode_s *(*iter_fn)(struct dnode_s *, void *), void *arg)
{
	struct dnode_s *node, *val;

	node = list->head->next;
	
	while (node->next) {
		val = iter_fn(node, arg);
		
		if (val)
			return val;
			
		node = node->next;
	}
	
	return 0;
}
