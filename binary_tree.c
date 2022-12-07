#include <stdlib.h>
#include "maze.h"
#include "algorithms.h"

static const enum MazeDirection bias[2] = {
	MAZE_DIRECTION_EAST,
	MAZE_DIRECTION_NORTH
};

static void
maze_alg_binary_tree_branch(struct Maze *maze, int x, int y)
{
	int i, ox, oy, order[2];
	enum MazeDirection dir;

	order[0] = rand() % 2;
	order[1] = 1 - order[0];

	for (i = 0; i < 2; ++i) {
		dir = bias[order[i]];
		maze_direction_offset(dir, &ox, &oy);

		if ((maze->data[y*maze->width+x] & dir) &&
				!maze_is_out_of_bounds(maze, x+ox, y+oy)) {
			maze->data[y*maze->width+x] ^= dir;
			maze->data[(y+oy)*maze->width+x+ox] ^= maze_direction_opposite(dir);
			break;
		}
	}
}

extern struct Maze *
maze_create_binary_tree(int width, int height, int seed)
{
	int x, y;
	struct Maze *maze;

	srand(seed);

	maze = maze_create(width, height, seed);
	maze->name = "Binary Tree";
	maze_fill(maze, MAZE_DIRECTION_ALL);

	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
			maze_alg_binary_tree_branch(maze, x, y);

	return maze;
}
