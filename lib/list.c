/* file:          list.c
 * description:   singly linked list data structure implementation
 * date:          04/2021
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

struct list_s *ucx_list_create(void)
{
	struct list_s *lst;

	lst = (struct list_s *)malloc(sizeof(struct list_s));

	if (lst) {
		lst->next = 0;
		lst->elem = 0;
	}

	return lst;
}

int32_t ucx_list_destroy(struct list_s *lst)
{
	if (lst->next)
		return -1;

	free(lst);

	return 0;
}

int32_t ucx_list_add(struct list_s *lst, void *item)
{
	struct list_s *t1, *t2;

	t1 = (struct list_s *)malloc(sizeof(struct list_s));

	if (t1) {
		t1->elem = item;
		t1->next = 0;
		t2 = lst;

		while (t2->next)
			t2 = t2->next;

		t2->next = t1;

		return 0;
	} else {
		return -1;
	}
}

void *ucx_list_peek(struct list_s *lst, int32_t pos)
{
	struct list_s *t1;
	int32_t i = 0;

	t1 = lst;
	while (t1->next) {
		t1 = t1->next;
		if (i++ == pos)
			return t1->elem;
	}

	return 0;
}

int32_t ucx_list_poke(struct list_s *lst, void *item, int32_t pos)
{
	struct list_s *t1;
	int32_t i = 0;

	t1 = lst;
	while (t1->next) {
		t1 = t1->next;
		if (i++ == pos) {
			t1->elem = item;
			
			return 0;
		}
	}

	return -1;
}

int32_t ucx_list_count(struct list_s *lst)
{
	struct list_s *t1;
	int32_t i = 0;

	t1 = lst;
	while (t1->next) {
		t1 = t1->next;
		i++;
	}

	return i;
}

int32_t ucx_list_insert(struct list_s *lst, void *item, int32_t pos)
{
	struct list_s *t1, *t2;
	int32_t i = 0;

	t1 = (struct list_s *)malloc(sizeof(struct list_s));

	if (t1) {
		t1->elem = item;
		t2 = lst;

		while (t2->next) {
			if (i++ == pos) break;
			t2 = t2->next;
		}

		t1->next = t2->next;
		t2->next = t1;

		return 0;
	} else {
		return -1;
	}
}

int32_t ucx_list_remove(struct list_s *lst, int32_t pos)
{
	struct list_s *t1, *t2;
	int32_t i = 0;

	t1 = lst;
	t2 = t1;

	while (t1->next) {
		t1 = t1->next;
		if (i++ == pos) {
			t2->next = t1->next;
			free(t1);

			return 0;
		}
		t2 = t1;
	}

	return -1;
}
