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

static void hunt(maze_t *m);

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
count_visited_nbors(maze_t *m, int x, int y)
{
	size_t i;
	maze_wall_t *nbor, walls[4];
	int visited_nbors;

	walls[0] = MAZE_WALL_NORTH;
	walls[1] = MAZE_WALL_SOUTH;
	walls[2] = MAZE_WALL_WEST;
	walls[3] = MAZE_WALL_EAST;

	visited_nbors = 0;

	for (i = 0; i < LEN(walls); ++i) {
		if ((nbor = maze_get_nbor_sharing_wall(m, x, y, walls[i]))
				&& *nbor != MAZE_WALL_ALL)
			visited_nbors += 1;
	}

	return visited_nbors;
}

static int
count_unvisited_nbors(maze_t *m, int x, int y)
{
	size_t i;
	maze_wall_t *nbor, walls[4];
	int unvisited_nbors;

	walls[0] = MAZE_WALL_NORTH;
	walls[1] = MAZE_WALL_WEST;
	walls[2] = MAZE_WALL_SOUTH;
	walls[3] = MAZE_WALL_EAST;

	unvisited_nbors = 0;

	for (i = 0; i < LEN(walls); ++i) {
		if ((nbor = maze_get_nbor_sharing_wall(m, x, y, walls[i]))
				&& *nbor == MAZE_WALL_ALL)
			unvisited_nbors += 1;
	}

	return unvisited_nbors;
}

static void
walk(maze_t *m, int x, int y)
{
	size_t i;
	int nx, ny;
	maze_wall_t *nbor, walls[4];

	if (count_unvisited_nbors(m, x, y) == 0) {
		hunt(m);
		return;
	}

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
			walk(m, nx, ny);
			break;
		}
	}
}

static void
hunt(maze_t *m)
{
	size_t i;
	int x, y;
	int nx, ny;
	maze_wall_t *nbor, walls[4];

	walls[0] = MAZE_WALL_NORTH;
	walls[1] = MAZE_WALL_WEST;
	walls[2] = MAZE_WALL_SOUTH;
	walls[3] = MAZE_WALL_EAST;

	for (y = 0; y < m->height; ++y) {
		for (x = 0; x < m->width; ++x) {
			if (*maze_get(m, x, y) != MAZE_WALL_ALL ||
					count_visited_nbors(m, x, y) == 0)
				continue;

			for (i = 0; i < LEN(walls); ++i) {
				if ((nbor = maze_get_nbor_sharing_wall(m, x, y, walls[i]))
						&& *nbor != MAZE_WALL_ALL) {
					maze_remove_wall(m, x, y, walls[i]);
					walk(m, x, y);
					return;
				}
			}
		}
	}
}

extern void
mazealg_hunt_and_kill(maze_t *m, int w, int h, int seed)
{
	maze_init(m, "hunt_and_kill", w, h, seed);
	walk(m, rand()%w, rand()%h);
}
