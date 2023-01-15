#include <stdlib.h>
#include "maze.h"
#include "algorithms.h"
#include "ll.h"

struct Point {
	int x;
	int y;
};

static void _maze_alg_hunt_and_kill_walk(struct Maze *maze, int x, int y);

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
_maze_get_visited_neighbours(struct Maze *maze, int x, int y)
{
	int count, i, ox, oy;
	const enum MazeDirection dirs[4] = {
		MAZE_DIRECTION_NORTH, MAZE_DIRECTION_SOUTH, MAZE_DIRECTION_WEST, MAZE_DIRECTION_EAST
	};

	count = 0;
	for (i = 0; i < 4; ++i) {
		maze_direction_offset(dirs[i], &ox, &oy);
		if (!maze_is_out_of_bounds(maze, x+ox, y+oy))
			if (maze->data[(y+oy)*maze->width+x+ox] != MAZE_DIRECTION_ALL)
				++count;
	}

	return count;
}

static int
_maze_has_available_neighbours(struct Maze *maze, int x, int y)
{
	int i, ox, oy;
	const enum MazeDirection dirs[4] = {
		MAZE_DIRECTION_NORTH, MAZE_DIRECTION_SOUTH, MAZE_DIRECTION_WEST, MAZE_DIRECTION_EAST
	};

	for (i = 0; i < 4; ++i) {
		maze_direction_offset(dirs[i], &ox, &oy);
		if (maze_is_out_of_bounds(maze, x+ox, y+oy)) continue;
		if (maze->data[(y+oy)*maze->width+x+ox] == MAZE_DIRECTION_ALL) return 1;
	}

	return 0;
}

static void
_maze_alg_hunt_and_kill_hunt(struct Maze *maze)
{
	int x, y, ox, oy, i;
	enum MazeDirection dir;
	const enum MazeDirection dirs[4] = {
		MAZE_DIRECTION_NORTH, MAZE_DIRECTION_SOUTH, MAZE_DIRECTION_WEST, MAZE_DIRECTION_EAST
	};

	for (y = 0; y < maze->height; ++y) {
		for (x = 0; x < maze->width; ++x) {
			if (maze->data[y*maze->width+x] == MAZE_DIRECTION_ALL &&
					_maze_get_visited_neighbours(maze, x, y) > 0) {
				for (i = 0; i < 4; ++i) {
					dir = dirs[i];
					maze_direction_offset(dir, &ox, &oy);

					if (maze_is_out_of_bounds(maze, x + ox, y + oy)) continue;
					if (maze->data[(y+oy)*maze->width+x+ox] != MAZE_DIRECTION_ALL) {
						maze->data[y*maze->width+x] ^= dir;
						maze->data[(y+oy)*maze->width+x+ox] ^= maze_direction_opposite(dir);

						_maze_alg_hunt_and_kill_walk(maze, x, y);
						return;
					}
				}
			}
		}
	}
}

static void
_maze_alg_hunt_and_kill_walk(struct Maze *maze, int x, int y)
{
	int i, ox, oy;
	enum MazeDirection dir;
	enum MazeDirection dirs[4];

	maze_random_directions(dirs);

	if (_maze_has_available_neighbours(maze, x, y)) {
		for (i = 0; i < 4; ++i) {
			dir = dirs[i];
			maze_direction_offset(dir, &ox, &oy);

			if (!maze_is_out_of_bounds(maze, x + ox, y + oy) &&
					maze->data[(oy+y)*maze->width+x+ox] == MAZE_DIRECTION_ALL) {
				maze->data[y*maze->width+x] ^= dir;
				maze->data[(y+oy)*maze->width+x+ox] ^= maze_direction_opposite(dir);

				_maze_alg_hunt_and_kill_walk(maze, x + ox, y + oy);
				break;
			}
		}
	} else _maze_alg_hunt_and_kill_hunt(maze);
}

extern struct Maze *
maze_create_hunt_and_kill(int width, int height, int seed)
{
	struct Maze *maze;

	srand(seed);

	maze = maze_create(width, height, seed);
	maze->name = "Hunt and Kill";
	maze_fill(maze, MAZE_DIRECTION_ALL);

	_maze_alg_hunt_and_kill_walk(maze, 0, 0);

	return maze;
}
