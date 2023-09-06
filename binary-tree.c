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

extern void
mazealg_binary_tree(maze_t *m, int w, int h, int seed)
{
	size_t i;
	int x, y, nx, ny;
	maze_wall_t *home, *nbor, bias[2];

	bias[0] = MAZE_WALL_EAST;
	bias[1] = MAZE_WALL_NORTH;

	maze_init(m, "binary_tree", w, h, seed);

	for (y = 0; y < m->height; ++y) {
		for (x = 0; x < m->width; ++x) {
			shuffle(bias, LEN(bias), sizeof(bias[0]));

			home = maze_get(m, x, y);

			for (i = 0; i < LEN(bias); ++i) {
				nx = x + MAZE_WALL_OFFSET_X(bias[i]);
				ny = y + MAZE_WALL_OFFSET_Y(bias[i]);

				if (*home & bias[i]
						&& (nbor = maze_get(m, nx, ny))) {
					maze_remove_wall(m, x, y, bias[i]);
					break;
				}
			}
		}
	}
}
