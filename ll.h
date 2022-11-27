#ifndef __LIBMAZE_LINKEDLIST_H__
#define __LIBMAZE_LINKEDLIST_H__

struct LinkedList {
	struct LinkedList *next;
	void *data;
};

extern void
linkedlist_append(struct LinkedList **ref, void *data);

extern void
linkedlist_join(struct LinkedList *head, struct LinkedList *list);

extern struct LinkedList *
linkedlist_tail(struct LinkedList *list);

extern void *
linkedlist_get(struct LinkedList *list, int pos);

extern void
linkedlist_remove_at(struct LinkedList **head, int pos);

extern int
linkedlist_length(struct LinkedList *list);

extern void
linkedlist_free(struct LinkedList *list);

#endif
