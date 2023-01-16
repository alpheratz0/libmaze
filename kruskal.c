#include <stdlib.h>
#include "maze.h"
#include "algorithms.h"
#include "ll.h"

struct Edge {
	int x;
	int y;
	enum MazeDirection dir;
};

struct Point {
	int x;
	int y;
};

static struct Edge *
_edge_create(int x, int y, enum MazeDirection dir)
{
	struct Edge *edge;
	edge = malloc(sizeof(struct Edge));
	edge->x = x;
	edge->y = y;
	edge->dir = dir;
	return edge;
}

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
maze_create_kruskal(int width, int height, int seed)
{
	struct Maze *maze;
	struct LinkedList *edges, **sets, *set1, *set2;
	struct Edge *edge;
	int x, y, ox, oy, edge_index;
	enum MazeDirection dir;

	srand(seed);

	maze = maze_create(width, height, seed);
	maze->name = "kruskal";
	maze_fill(maze, MAZE_DIRECTION_ALL);
	sets = calloc(width * height, sizeof(struct LinkedList *));
	edges = NULL;

	for (y = 0; y < maze->height; ++y) {
		for (x = 0; x < maze->width; ++x) {
			linkedlist_append(&sets[y*maze->width+x], _point_create(x, y));
			if (x > 0) linkedlist_append(&edges, _edge_create(x, y, MAZE_DIRECTION_WEST));
			if (y > 0) linkedlist_append(&edges, _edge_create(x, y, MAZE_DIRECTION_NORTH));
		}
	}

	while (linkedlist_length(edges) > 0) {
		edge_index = rand() % linkedlist_length(edges);
		edge = linkedlist_get(edges, edge_index);
		x = edge->x;
		y = edge->y;
		dir = edge->dir;
		maze_direction_offset(dir, &ox, &oy);

		set1 = sets[y*maze->width+x];
		set2 = sets[(y+oy)*maze->width+x+ox];

		if (linkedlist_tail(set1) != linkedlist_tail(set2)) {
			linkedlist_join(set1, set2);
			maze->data[y*maze->width+x] ^= dir;
			maze->data[(y+oy)*maze->width+x+ox] ^= maze_direction_opposite(dir);
		}

		linkedlist_remove_at(&edges, edge_index);
	}

	return maze;
}
