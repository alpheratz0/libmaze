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
#include <string.h>
#include "maze.h"

#define LEN(arr) (sizeof(arr)/sizeof(arr[0]))

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

static void
backtrack(maze_t *m, int x, int y)
{
	size_t i;
	int nx, ny;
	maze_wall_t *nbor, walls[4];

	walls[0] = MAZE_WALL_NORTH;
	walls[1] = MAZE_WALL_WEST;
	walls[2] = MAZE_WALL_SOUTH;
	walls[3] = MAZE_WALL_EAST;

	shuffle(walls, LEN(walls), sizeof(walls[0]));

	for (i = 0; i < LEN(walls); ++i) {
		nx = x + MAZE_WALL_OFFSET_X(walls[i]);
		ny = y + MAZE_WALL_OFFSET_Y(walls[i]);

		if ((nbor = maze_get(m, nx, ny))
				&& *nbor == MAZE_WALL_ALL) {
			maze_remove_wall(m, x, y, walls[i]);
			backtrack(m, nx, ny);
		}
	}
}

extern void
maze_recursive_backtracking(maze_t *m, int w, int h, int seed)
{
	maze_init(m, "recursive_backtracking", w, h, seed);
	backtrack(m, rand()%m->width, rand()%m->height);
}
