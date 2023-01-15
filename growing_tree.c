#include <stdlib.h>
#include "maze.h"
#include "algorithms.h"
#include "ll.h"

struct Point {
	int x;
	int y;
};

static struct Point *
_point_create(int x, int y)
{
	struct Point *point;
	point = malloc(sizeof(struct Point));
	point->x = x;
	point->y = y;
	return point;
}

static int
_maze_has_available_neighbours(struct Maze *maze, int x, int y)
{
	int i, ox, oy;
	static const enum MazeDirection dirs[4] = {
		MAZE_DIRECTION_NORTH, MAZE_DIRECTION_SOUTH, MAZE_DIRECTION_WEST, MAZE_DIRECTION_EAST
	};

	for (i = 0; i < 4; ++i) {
		maze_direction_offset(dirs[i], &ox, &oy);
		if (maze_is_out_of_bounds(maze, x+ox, y+oy)) continue;
		if (maze->data[(y+oy)*maze->width+x+ox] == MAZE_DIRECTION_ALL) return 1;
	}

	return 0;
}

extern struct Maze *
maze_create_growing_tree(int width, int height, int seed)
{
	struct Maze *maze;
	struct LinkedList *cells;
	struct Point *pt;
	enum MazeDirection dirs[4], dir;
	int i, ox, oy, pt_index;

	srand(seed);

	maze = maze_create(width, height, seed);
	maze->name = "Growing Tree";
	maze_fill(maze, MAZE_DIRECTION_ALL);
	cells = NULL;

	linkedlist_append(&cells, _point_create(rand() % width, rand() % height));

	while (linkedlist_length(cells) > 0) {
		pt_index = rand() % linkedlist_length(cells);
		pt = linkedlist_get(cells, pt_index);
		maze_random_directions(dirs);

		for (i = 0; i < 4; ++i) {
			dir = dirs[i];
			maze_direction_offset(dir, &ox, &oy);

			if (maze_is_out_of_bounds(maze, pt->x + ox, pt->y + oy)) continue;
			if (maze->data[(pt->y+oy)*maze->width+pt->x+ox] != MAZE_DIRECTION_ALL) continue;

			maze->data[pt->y*maze->width+pt->x] ^= dir;
			maze->data[(pt->y+oy)*maze->width+pt->x+ox] ^= maze_direction_opposite(dir);

			linkedlist_append(&cells, _point_create(pt->x+ox, pt->y+oy));
			break;
		}


		if (!_maze_has_available_neighbours(maze, pt->x, pt->y))
			linkedlist_remove_at(&cells, pt_index);
	}

	return maze;
}
