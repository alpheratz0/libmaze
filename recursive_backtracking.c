#include <stdlib.h>
#include "maze.h"
#include "algorithms.h"

static void
maze_alg_recursive_backtracking_backtrack(struct Maze *maze, int x, int y)
{
	int i, ox, oy;
	enum MazeDirection dirs[4];

	maze_random_directions(dirs);

	for (i = 0; i < 4; ++i) {
		maze_direction_offset(dirs[i], &ox, &oy);
		if (maze_is_out_of_bounds(maze, x+ox, y+oy)) continue;
		if (maze->data[(y+oy)*maze->width+x+ox] != MAZE_DIRECTION_ALL) continue;
		maze->data[y*maze->width+x] ^= dirs[i];
		maze->data[(y+oy)*maze->width+x+ox] ^= maze_direction_opposite(dirs[i]);
		maze_alg_recursive_backtracking_backtrack(maze, x+ox, y+oy);
	}
}

extern struct Maze *
maze_alg_recursive_backtracking(int width, int height, int seed)
{
	struct Maze *maze;

	srand(seed);

	maze = maze_create(width, height, seed);
	maze->name = "Recursive Backtracking";
	maze_fill(maze, MAZE_DIRECTION_ALL);
	maze_alg_recursive_backtracking_backtrack(maze, 0, 0);

	return maze;
}
