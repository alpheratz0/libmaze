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

enum {
	MAZE_WALL_NONE   = (0),
	MAZE_WALL_NORTH  = (1 << 0),
	MAZE_WALL_WEST   = (1 << 1),
	MAZE_WALL_SOUTH  = (1 << 2),
	MAZE_WALL_EAST   = (1 << 3),
	MAZE_WALL_ALL    = (1 << 4) - 1
};

#define MAZE_WALL_OPPOSITE(wall) \
	((wall<<2)|(wall>>2))&0xf

#define MAZE_WALL_OFFSET_X(wall) \
	((wall==MAZE_WALL_EAST)-(wall==MAZE_WALL_WEST))

#define MAZE_WALL_OFFSET_Y(wall) \
	((wall==MAZE_WALL_SOUTH)-(wall==MAZE_WALL_NORTH))

#define MAZE_WALL_BY_OFFSET(offx,offy) \
	(((offx<0)*MAZE_WALL_WEST) | \
	 ((offx>0)*MAZE_WALL_EAST) | \
	 ((offy<0)*MAZE_WALL_NORTH) | \
	 ((offy>0)*MAZE_WALL_SOUTH))

typedef unsigned char maze_wall_t;
typedef struct maze maze_t;
typedef void (*maze_algorithm_t)(maze_t *m, int w, int h, int seed);

struct maze {
	const char *name;
	int width, height;
	int seed;
	maze_wall_t *data;
};

extern void
maze_init(maze_t *maze, const char *name, int w, int h, int seed);

extern maze_wall_t *
maze_get(maze_t *m, int x, int y);

extern maze_wall_t *
maze_get_nbor_sharing_wall(maze_t *m, int x, int y, maze_wall_t wall);

extern void
maze_remove_wall(maze_t *m, int x, int y, maze_wall_t wall);

extern void
maze_fini(maze_t *maze);
