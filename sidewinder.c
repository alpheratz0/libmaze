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

#include <stdlib.h>

#include "util/dynarr.h"
#include "maze.h"

typedef struct {
	int x;
	int y;
} point_t;

static point_t *
point_new(int x, int y)
{
	point_t *pt;
	pt = malloc(sizeof(point_t));
	pt->x = x;
	pt->y = y;
	return pt;
}

extern void
mazealg_sidewinder(maze_t *m, int w, int h, int seed)
{
	dynarr_t *path;
	point_t *pt;
	int x, y;

	maze_init(m, "sidewinder", w, h, seed);
	path = NULL;

	for (y = 0; y < m->height; ++y, dynarr_free2(&path, free)) {
		for (x = 0; x < m->width; ++x) {
			dynarr_push2(&path, point_new(x, y));

			if (y == m->height-1) { // bottom line
				if (x != m->width-1) // bottom right
					maze_remove_wall(m, x, y, MAZE_WALL_EAST);
			} else if (x != m->width-1 && (rand()%2)) {
				maze_remove_wall(m, x, y, MAZE_WALL_EAST);
			} else {
				pt = dynarr_get(path, rand()%path->len);
				maze_remove_wall(m, pt->x, pt->y, MAZE_WALL_SOUTH);
				dynarr_free2(&path, free);
			}
		}
	}
}
