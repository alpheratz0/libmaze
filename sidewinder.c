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

extern struct Maze *
maze_create_sidewinder(int width, int height, int seed)
{
	struct Maze *maze;
	struct LinkedList *path;
	struct Point *pt;
	int x, y;

	srand(seed);

	maze = maze_create(width, height, seed);
	maze->name = "Sidewinder";
	maze_fill(maze, MAZE_DIRECTION_ALL);
	path = NULL;

	for (y = 0; y < maze->height; ++y) {
		for (x = 0; x < maze->width; ++x) {
			linkedlist_append(&path, _point_create(x, y));
			if (y == maze->height - 1) {
				if (x != maze->width - 1) {
					maze->data[y*maze->width+x] ^= MAZE_DIRECTION_EAST;
					maze->data[y*maze->width+x+1] ^= MAZE_DIRECTION_WEST;
				}
			} else if (x != maze->width - 1 && (rand() % 2)) {
				maze->data[y*maze->width+x] ^= MAZE_DIRECTION_EAST;
				maze->data[y*maze->width+x+1] ^= MAZE_DIRECTION_WEST;
				continue;
			} else {
				pt = linkedlist_get(path, rand() % linkedlist_length(path));
				maze->data[pt->y*maze->width+pt->x] ^= MAZE_DIRECTION_SOUTH;
				maze->data[(pt->y+1)*maze->width+pt->x] ^= MAZE_DIRECTION_NORTH;
				linkedlist_free(path);
				path = NULL;
			}
		}
		if (NULL != path) {
			linkedlist_free(path);
			path = NULL;
		}
	}

	return maze;
}
