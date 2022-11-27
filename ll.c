#include <stdlib.h>

#include "ll.h"

extern void
linkedlist_append(struct LinkedList **ref, void *data)
{
	while (NULL != ref[0])
		ref = &ref[0]->next;
	ref[0] = malloc(sizeof(struct LinkedList));
	ref[0]->data = data;
	ref[0]->next = NULL;
}

extern void
linkedlist_join(struct LinkedList *head, struct LinkedList *list)
{
	struct LinkedList *tail;
	if (!list) return;
	tail = head;
	while (NULL != tail->next)
		tail = tail->next;
	tail->next = list;
}

extern struct LinkedList *
linkedlist_tail(struct LinkedList *list)
{
	while (list->next)
		list = list->next;
	return list;
}

extern void *
linkedlist_get(struct LinkedList *list, int pos)
{
	while (pos-- > 0 && NULL != list)
		list = list->next;
	return list->data;
}

extern void
linkedlist_remove_at(struct LinkedList **head, int pos)
{
	struct LinkedList *target, *prev;
	if (pos == 0) {
		target = *head;
		*head = target->next;
		free(target->data);
		free(target);
		return;
	}
	target = *head;
	prev = NULL;
	while (pos-- > 0 && NULL != target)
		prev = target, target = target->next;
	prev->next = target->next;
	free(target->data);
	free(target);
}

extern int
linkedlist_length(struct LinkedList *list)
{
	int length;

	for (length = 0; NULL != list; ++length)
		list = list->next;

	return length;
}

extern void
linkedlist_free(struct LinkedList *list)
{
	struct LinkedList *head, *temp;

	head = list;

	while (head) {
		temp = head;
		head = head->next;
		free(temp->data);
		free(temp);
	}
}
