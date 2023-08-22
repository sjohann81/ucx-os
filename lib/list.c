/* file:          list.c
 * description:   doubly linked list data structure implementation
 * date:          08/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct list_s *list_create()
{
	struct list_s *list;
	struct node_s *head, *tail;
	
	list = malloc(sizeof(struct node_s));
	
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
	
	head->prev = 0;
	head->next = tail;
	head->data = (void *)0xbaadf00d;
	tail->prev = head;
	tail->next = 0;
	tail->data = (void *)0xdeadbeef;
	
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
	node->prev = list->head;
	node->next = list->head->next;	
	list->head->next->prev = node;
	list->head->next = node;
	list->length++;
	
	return node;
}

struct node_s *list_pushback(struct list_s *list, void *val)
{
	struct node_s *node;
	
	node = malloc(sizeof(struct node_s));
	
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

void *list_pop(struct list_s *list)
{
	struct node_s *node;
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

void *list_popback(struct list_s *list)
{
	struct node_s *node;
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

struct node_s *list_insert(struct list_s *list, struct node_s *prevnode, void *val)
{
	struct node_s *node;
	
	node = malloc(sizeof(struct node_s));
	
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

struct node_s *list_remove(struct list_s *list, struct node_s *node)
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

struct node_s *list_index(struct list_s *list, int idx)
{
	struct node_s *node;
	
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
