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

#include <string.h>
#include <stdlib.h>
#include "maze.h"

extern void
maze_init(maze_t *m, const char *name, int w, int h, int seed)
{
	m->name = name;
	m->width = w;
	m->height = h;
	m->seed = seed;
	m->data = malloc(w*h);
	memset(m->data, MAZE_WALL_ALL, w*h);
	srand(seed);
}

extern maze_wall_t *
maze_get(maze_t *m, int x, int y)
{
	if (x >= 0 && x < m->width && y >= 0 && y < m->height)
		return &m->data[y*m->width+x];
	return NULL;
}

extern maze_wall_t *
maze_get_nbor_sharing_wall(maze_t *m, int x, int y, maze_wall_t wall)
{
	maze_wall_t *nbor;
	int nbor_x, nbor_y;

	nbor_x = x + MAZE_WALL_OFFSET_X(wall);
	nbor_y = y + MAZE_WALL_OFFSET_Y(wall);

	nbor = maze_get(m, nbor_x, nbor_y);

	return nbor;
}

extern void
maze_remove_wall(maze_t *m, int x, int y, maze_wall_t wall)
{
	maze_wall_t *target, *nbor;

	target = maze_get(m, x, y);
	nbor = maze_get_nbor_sharing_wall(m, x, y, wall);

	*target ^= wall;
	*nbor ^= MAZE_WALL_OPPOSITE(wall);
}

extern void
maze_fini(maze_t *maze)
{
	free(maze->data);
	memset(maze, 0, sizeof(maze_t));
}
