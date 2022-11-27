#ifndef __LIBMAZE_ALGORITHMS_H__
#define __LIBMAZE_ALGORITHMS_H__

#include "maze.h"

extern struct Maze *
maze_create_recursive_backtracking(int width, int height, int seed);

extern struct Maze *
maze_create_sidewinder(int width, int height, int seed);

#endif
