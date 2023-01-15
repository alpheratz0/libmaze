#ifndef __LIBMAZE_ALGORITHMS_H__
#define __LIBMAZE_ALGORITHMS_H__

#include "maze.h"

extern struct Maze *
maze_create_recursive_backtracking(int width, int height, int seed);

extern struct Maze *
maze_create_sidewinder(int width, int height, int seed);

extern struct Maze *
maze_create_binary_tree(int width, int height, int seed);

extern struct Maze *
maze_create_growing_tree(int width, int height, int seed);

extern struct Maze *
maze_create_kruskal(int width, int height, int seed);

extern struct Maze *
maze_create_hunt_and_kill(int width, int height, int seed);

#endif
