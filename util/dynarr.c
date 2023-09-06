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

#include <stddef.h>
#include <stdlib.h>
#include "dynarr.h"

extern dynarr_t *
dynarr_new(void)
{
	dynarr_t *dynarr;

	dynarr = malloc(sizeof(dynarr_t));

	dynarr->len = 0;
	dynarr->cap = 16;
	dynarr->elements = malloc(
		dynarr->cap*sizeof(void*));

	return dynarr;
}

extern void
dynarr_push(dynarr_t *dynarr, void *elem)
{
	if (dynarr->len == dynarr->cap) {
		dynarr->cap += 16;
		dynarr->elements = realloc(dynarr->elements,
				dynarr->cap*sizeof(void*));
	}

	dynarr->elements[dynarr->len] = elem;
	dynarr->len += 1;
}

extern void
dynarr_push2(dynarr_t **dynarr, void *elem)
{
	if (!*dynarr)
		*dynarr = dynarr_new();
	dynarr_push(*dynarr, elem);
}

extern void *
dynarr_get(dynarr_t *dynarr, int index)
{
	if (index < 0 || index >= (int)(dynarr->len))
		return NULL;
	return dynarr->elements[index];
}

extern void *
dynarr_shift(dynarr_t *dynarr)
{
	size_t i;
	void *elem;

	if (dynarr->len == 0)
		return NULL;

	elem = dynarr->elements[0];
	dynarr->len -= 1;

	for (i = 0; i < dynarr->len; ++i)
		dynarr->elements[i] = dynarr->elements[i+1];

	return elem;
}

extern void *
dynarr_pop(dynarr_t *dynarr)
{
	if (dynarr->len == 0)
		return NULL;

	dynarr->len -= 1;
	return dynarr->elements[dynarr->len];
}

extern void *
dynarr_remove_at(dynarr_t *dynarr, int index)
{
	size_t i;
	void *elem;

	if (index < 0 || index >= (int)(dynarr->len))
		return NULL;

	elem = dynarr->elements[index];
	dynarr->len -= 1;

	for (i = index; i < dynarr->len; ++i)
		dynarr->elements[i] = dynarr->elements[i+1];

	return elem;
}

extern void *
dynarr_remove(dynarr_t *dynarr, void *elem)
{
	size_t i;
	for (i = 0; i < dynarr->len; ++i)
		if (dynarr->elements[i] == elem)
			return dynarr_remove_at(dynarr, i);
	return NULL;
}

extern void
dynarr_foreach(dynarr_t *dynarr, dynarr_action_t act)
{
	size_t i;
	for (i = 0; i < dynarr->len; ++i)
		act(dynarr->elements[i]);
}

extern void
dynarr_free(dynarr_t *dynarr, dynarr_action_t act)
{
	if (!dynarr) return;
	dynarr_foreach(dynarr, act);
	free(dynarr->elements);
	free(dynarr);
}

extern void
dynarr_free2(dynarr_t **dynarr, dynarr_action_t act)
{
	dynarr_free(*dynarr, act);
	*dynarr = NULL;
}
