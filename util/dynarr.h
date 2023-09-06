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

#include <stddef.h>

typedef struct {
	size_t len;
	size_t cap;
	void **elements;
} dynarr_t;

typedef void (*dynarr_action_t)(void *elem);

extern dynarr_t *
dynarr_new(void);

extern void
dynarr_push(dynarr_t *dynarr, void *elem);

extern void
dynarr_push2(dynarr_t **dynarr, void *elem);

extern void *
dynarr_get(dynarr_t *dynarr, int index);

extern void *
dynarr_shift(dynarr_t *dynarr);

extern void *
dynarr_pop(dynarr_t *dynarr);

extern void *
dynarr_remove_at(dynarr_t *dynarr, int index);

extern void
dynarr_foreach(dynarr_t *dynarr, dynarr_action_t act);

extern void
dynarr_free(dynarr_t *dynarr, dynarr_action_t act);

extern void
dynarr_free2(dynarr_t **dynarr, dynarr_action_t act);
