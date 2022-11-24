#ifndef __LIBMAZE_MAZE_H__
#define __LIBMAZE_MAZE_H__

enum MazeDirection {
	MAZE_DIRECTION_NONE   = (0),
	MAZE_DIRECTION_NORTH  = (1 << 0),
	MAZE_DIRECTION_WEST   = (1 << 1),
	MAZE_DIRECTION_SOUTH  = (1 << 2),
	MAZE_DIRECTION_EAST   = (1 << 3),
	MAZE_DIRECTION_ALL    = (1 << 4) - 1
};

struct Maze {
	const char *name;
	int width;
	int height;
	int seed;
	enum MazeDirection *data;
};

extern struct Maze *
maze_create(int width, int height, int seed);

extern enum MazeDirection
maze_direction_opposite(enum MazeDirection mdir);

extern void
maze_direction_offset(enum MazeDirection mdir, int *xoff, int *yoff);

extern void
maze_offset_direction(int xoff, int yoff, enum MazeDirection *mdir);

extern int
maze_is_out_of_bounds(struct Maze *maze, int x, int y);

extern void
maze_fill(struct Maze *maze, enum MazeDirection mdir);

extern void
maze_random_directions(enum MazeDirection dirs[4]);

#endif
