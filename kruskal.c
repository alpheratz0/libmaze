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

#include <stdbool.h>
#include <stdlib.h>

#include "util/ll.h"
#include "util/dynarr.h"
#include "maze.h"

typedef struct {
	int x;
	int y;
} point_t;

typedef struct {
	int x, y;
	int nx, ny;
	maze_wall_t wall;
} edge_t;

static point_t *
point_new(int x, int y)
{
	point_t *pt;
	pt = malloc(sizeof(point_t));
	pt->x = x;
	pt->y = y;
	return pt;
}

static edge_t *
edge_new(int x, int y, maze_wall_t wall)
{
	edge_t *edge;
	edge = malloc(sizeof(edge_t));
	edge->x = x;
	edge->y = y;
	edge->wall = wall;
	edge->nx = x + MAZE_WALL_OFFSET_X(wall);
	edge->ny = y + MAZE_WALL_OFFSET_Y(wall);
	return edge;
}

extern void
maze_kruskal(maze_t *m, int w, int h, int seed)
{
	dynarr_t *edges, *paths;
	node_t *path1, *path2;
	edge_t *edge;
	int x, y;

	maze_init(m, "kruskal", w, h, seed);

	edges = dynarr_new();
	paths = dynarr_new();

	// generate a list of all available edges
	for (y = 0; y < m->height; ++y) {
		for (x = 0; x < m->width; ++x) {
			dynarr_push(paths, ll_node_new(point_new(x, y), true));
			if (x > 0) dynarr_push(edges, edge_new(x, y, MAZE_WALL_WEST));
			if (y > 0) dynarr_push(edges, edge_new(x, y, MAZE_WALL_NORTH));
		}
	}

	while (edges->len > 0) {
		// select random available edge
		edge  = dynarr_get(edges, rand()%edges->len);

		path1 = dynarr_get(paths, edge->y*m->width+edge->x);
		path2 = dynarr_get(paths, edge->ny*m->width+edge->nx);

		// if both paths are not connected
		if (!ll_share_nodes(path1, path2)) {
			// connect the paths
			ll_join(path1, path2);
			maze_remove_wall(m, edge->x, edge->y, edge->wall);
		}

		free(dynarr_remove(edges, edge));
	}

	dynarr_free2(&paths, (dynarr_action_t)(ll_node_free));
	dynarr_free2(&edges, NULL);
}
