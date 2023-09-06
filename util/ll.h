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

#pragma once

#include <stdbool.h>

typedef struct node node_t;
typedef struct node ll_t;

struct node {
	node_t *next;
	bool data_owned;
	void *data;
};

extern node_t *
ll_node_new(void *data, bool data_owned);

extern node_t *
ll_tail(ll_t *ll);

extern int
ll_share_nodes(ll_t *ll, ll_t *other);

extern void
ll_join(ll_t *ll, ll_t *other);

extern void
ll_node_free(node_t *node);
