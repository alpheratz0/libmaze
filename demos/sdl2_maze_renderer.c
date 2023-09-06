/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////
/////  compile with:
/////
/////  cc sdl2_maze_renderer.c ../*.c -o sdl2_maze_renderer -lSDL2
/////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "../maze.h"
#include "../algorithms.h"

#define MARGIN 20

static void
die(const char *fmt, ...)
{
	va_list args;

	fputs("sdl2_maze_renderer: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
	exit(1);
}

static void
usage(void)
{
	fprintf(stderr, "usage: sdl2_maze_renderer [-recursive_backtracking] [-sidewinder]\n"
					"                          [-binary_tree] [-growing_tree] [-kruskal]\n"
					"                          [-hunt_and_kill] [-rows rows]\n"
					"                          [-columns columns] [-seed seed]\n");
	exit(1);
}

static const char *
enotnull(const char *str, const char *name)
{
	if (NULL == str)
		die("%s cannot be null", name);
	return str;
}

static void
rect_to_points(int x, int y, int w, int h, SDL_Point *points)
{
	points[0].x = x;
	points[0].y = y;
	points[1].x = x + w;
	points[1].y = y;
	points[2].x = x + w;
	points[2].y = y + h;
	points[3].x = x;
	points[3].y = y + h;
}

int
main(int argc, char **argv)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int x, y;
	int width, height;
	int cell_width, cell_height;
	int used_width, used_height;
	int unused_width, unused_height;
	int rows, columns, seed;
	SDL_Point pts[4];
	SDL_Event ev;
	maze_algorithm_t alg;
	maze_t maze;

	columns = rows = 50;
	seed = 5050;
	alg = mazealg_recursive_backtracking;

	while (++argv, --argc > 0) {
		if (!strcmp(*argv, "-recursive_backtracking")) {
			alg = mazealg_recursive_backtracking;
		} else if (!strcmp(*argv, "-sidewinder")) {
			alg = mazealg_sidewinder;
		} else if (!strcmp(*argv, "-binary_tree")) {
			alg = mazealg_binary_tree;
		} else if (!strcmp(*argv, "-growing_tree")) {
			alg = mazealg_growing_tree;
		} else if (!strcmp(*argv, "-kruskal")) {
			alg = mazealg_kruskal;
		} else if (!strcmp(*argv, "-hunt_and_kill")) {
			alg = mazealg_hunt_and_kill;
		} else if (!strcmp(*argv, "-rows")) {
			--argc; rows = atoi(enotnull(*++argv, "rows"));
		} else if (!strcmp(*argv, "-columns")) {
			--argc; columns = atoi(enotnull(*++argv, "columns"));
		} else if (!strcmp(*argv, "-seed")) {
			--argc; seed = atoi(enotnull(*++argv, "seed"));
		} else {
			usage();
		}
	}

	alg(&maze, columns, rows, seed);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		die("SDL_Init failed: %s", SDL_GetError());

	window = SDL_CreateWindow("sdl2_maze_renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (NULL == window)
		die("SDL_CreateWindow failed: %s", SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (NULL == renderer)
		die("SDL_CreateRenderer failed: %s", SDL_GetError());

	while (1) {
		while (SDL_PollEvent(&ev))
			if (ev.type == SDL_QUIT)
				goto quit;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_GetWindowSize(window, &width, &height);

		width         -=  MARGIN * 2;
		height        -=  MARGIN * 2;
		cell_width     =  width / maze.width;
		cell_height    =  height / maze.height;
		used_width     =  cell_width * maze.width;
		used_height    =  cell_height * maze.height;
		unused_width   =  width - used_width;
		unused_height  =  height - used_height;

		for (y = 0; y < maze.height; ++y) {
			for (x = 0; x < maze.width; ++x) {
				rect_to_points(MARGIN + (x * cell_width) + unused_width / 2,
						       MARGIN + (y * cell_height) + unused_height / 2,
							   cell_width, cell_height, pts);

				if (maze.data[y*maze.width+x] & MAZE_WALL_NORTH)
					SDL_RenderDrawLine(renderer, pts[0].x, pts[0].y, pts[1].x, pts[1].y);

				if (maze.data[y*maze.width+x] & MAZE_WALL_SOUTH)
					SDL_RenderDrawLine(renderer, pts[3].x, pts[3].y, pts[2].x, pts[2].y);

				if (maze.data[y*maze.width+x] & MAZE_WALL_WEST)
					SDL_RenderDrawLine(renderer, pts[0].x, pts[0].y, pts[3].x, pts[3].y);

				if (maze.data[y*maze.width+x] & MAZE_WALL_EAST)
					SDL_RenderDrawLine(renderer, pts[1].x, pts[1].y, pts[2].x, pts[2].y);
			}
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 60);
	}

quit:
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	maze_fini(&maze);

	return 0;
}
