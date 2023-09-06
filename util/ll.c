// Copyright (C) 2023 <alpheratz99@protonmail.com>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include <stdbool.h>
#include <stdlib.h>

#include "ll.h"

extern node_t *
ll_node_new(void *data, bool data_owned)
{
	node_t *node;

	node = malloc(sizeof(node_t));

	node->next = NULL;
	node->data_owned = data_owned;
	node->data = data;

	return node;
}

extern node_t *
ll_tail(ll_t *ll)
{
	while (ll->next != NULL)
		ll = ll->next;
	return ll;
}

extern int
ll_share_nodes(ll_t *ll, ll_t *other)
{
	return ll_tail(ll) == ll_tail(other);
}

extern void
ll_join(ll_t *ll, ll_t *other)
{
	node_t *tail;
	if (ll_share_nodes(ll, other))
		return;
	tail = ll_tail(ll);
	tail->next = other;
}

extern void
ll_node_free(node_t *node)
{
	if (node->data_owned)
		free(node->data);
	free(node);
}
