#include <stdlib.h>
#include "maze.h"

extern struct Maze *
maze_create(int width, int height, int seed)
{
	struct Maze *maze;

	maze = malloc(sizeof(struct Maze));
	maze->width = width;
	maze->height = height;
	maze->seed = seed;
	maze->name = NULL;
	maze->data = calloc(width * height, sizeof(enum MazeDirection));

	return maze;
}

extern enum MazeDirection
maze_direction_opposite(enum MazeDirection mdir)
{
	switch (mdir) {
	case MAZE_DIRECTION_NONE:    return MAZE_DIRECTION_ALL;
	case MAZE_DIRECTION_NORTH:   return MAZE_DIRECTION_SOUTH;
	case MAZE_DIRECTION_WEST:    return MAZE_DIRECTION_EAST;
	case MAZE_DIRECTION_SOUTH:   return MAZE_DIRECTION_NORTH;
	case MAZE_DIRECTION_EAST:    return MAZE_DIRECTION_WEST;
	case MAZE_DIRECTION_ALL:     return MAZE_DIRECTION_NONE;
	default:                     return MAZE_DIRECTION_NONE;
	}
}

extern void
maze_direction_offset(enum MazeDirection mdir, int *xoff, int *yoff)
{
	*xoff = *yoff = 0;
	switch (mdir) {
	case MAZE_DIRECTION_NORTH:   *yoff = -1; break;
	case MAZE_DIRECTION_WEST:    *xoff = -1; break;
	case MAZE_DIRECTION_SOUTH:   *yoff =  1; break;
	case MAZE_DIRECTION_EAST:    *xoff =  1; break;
	default:                                 break;
	}
}

extern void
maze_offset_direction(int xoff, int yoff, enum MazeDirection *mdir)
{
	*mdir = MAZE_DIRECTION_NONE;
	if (xoff < 0) *mdir |= MAZE_DIRECTION_WEST;
	if (xoff > 0) *mdir |= MAZE_DIRECTION_EAST;
	if (yoff < 0) *mdir |= MAZE_DIRECTION_NORTH;
	if (yoff > 0) *mdir |= MAZE_DIRECTION_SOUTH;
}

extern int
maze_is_out_of_bounds(struct Maze *maze, int x, int y)
{
	if (x < 0 || x >= maze->width) return 1;
	if (y < 0 || y >= maze->height) return 1;
	return 0;
}

extern void
maze_fill(struct Maze *maze, enum MazeDirection mdir)
{
	int x, y;
	for (y = 0; y < maze->height; ++y)
		for (x = 0; x < maze->width; ++x)
			maze->data[y*maze->width+x] = mdir;
	for (y = 0; y < maze->height; ++y) {
		maze->data[(y  )*maze->width  ] |= MAZE_DIRECTION_WEST;
		maze->data[(y+1)*maze->width-1] |= MAZE_DIRECTION_EAST;
	}
	for (x = 0; x < maze->width; ++x) {
		maze->data[x]                              |= MAZE_DIRECTION_NORTH;
		maze->data[(maze->height-1)*maze->width+x] |= MAZE_DIRECTION_SOUTH;
	}
}

extern void
maze_random_directions(enum MazeDirection dirs[4])
{
	int i;
	enum MazeDirection dir, leftdirs;
	leftdirs = MAZE_DIRECTION_ALL;
	for (i = 0; i < 4;)
		if ((dir = (1 << (rand() % 4))) & leftdirs)
			dirs[i++] = dir, leftdirs ^= dir;
}

