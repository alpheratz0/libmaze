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

#define LEN(arr) (sizeof(arr)/sizeof(arr[0]))

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

static int
random_comparer(const void *p0, const void *p1)
{
	(void) p0; (void) p1;
	return rand() % 2 == 0 ? -1 : 1;
}

static void
shuffle(void *arr, size_t n, size_t size)
{
	qsort(arr, n, size, random_comparer);
}

static int
any_full_nbor(maze_t *m, int x, int y)
{
	size_t i;
	maze_wall_t *nbor, walls[4];

	walls[0] = MAZE_WALL_NORTH;
	walls[1] = MAZE_WALL_SOUTH;
	walls[2] = MAZE_WALL_WEST;
	walls[3] = MAZE_WALL_EAST;

	for (i = 0; i < LEN(walls); ++i) {
		if ((nbor = maze_get_nbor_sharing_wall(m, x, y, walls[i]))
				&& *nbor == MAZE_WALL_ALL)
			return 1;
	}

	return 0;
}

extern void
maze_growing_tree(maze_t *m, int w, int h, int seed)
{
	size_t i;
	dynarr_t *cells;
	maze_wall_t *nbor, walls[4];
	point_t *pt;
	int nx, ny;

	maze_init(m, "growing_tree", w, h, seed);

	cells = NULL;

	walls[0] = MAZE_WALL_NORTH;
	walls[1] = MAZE_WALL_WEST;
	walls[2] = MAZE_WALL_SOUTH;
	walls[3] = MAZE_WALL_EAST;

	dynarr_push2(&cells, point_new(rand()%w, rand()%h));

	while (cells->len > 0) {
		pt = dynarr_get(cells, rand()%cells->len);
		shuffle(walls, LEN(walls), sizeof(walls[0]));
		for (i = 0; i < LEN(walls); ++i) {
			nx = pt->x + MAZE_WALL_OFFSET_X(walls[i]);
			ny = pt->y + MAZE_WALL_OFFSET_Y(walls[i]);
			if ((nbor = maze_get(m, nx, ny))
					&& *nbor == MAZE_WALL_ALL) {
				maze_remove_wall(m, pt->x, pt->y, walls[i]);
				dynarr_push2(&cells, point_new(nx, ny));
				break;
			}
		}
		if (!any_full_nbor(m, pt->x, pt->y))
			free(dynarr_remove(cells, pt));
	}

	dynarr_free2(&cells, free);
}
