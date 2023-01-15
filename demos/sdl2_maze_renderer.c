/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//
// compile with:
//
// cc sdl2_maze_renderer.c ../*.c -o sdl2_maze_renderer -lSDL2
//
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "../maze.h"
#include "../algorithms.h"

#define MARGIN 20

static SDL_Window *window;
static SDL_Renderer *renderer;
static struct Maze *maze;

static void
dief(const char *fmt, ...)
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
					"                          [-hunt_and_kill]\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	int x, y, ax, ay, bx, by, cx, cy, dx, dy;
	int width, height;
	SDL_Event ev;

	if (++argv, --argc > 0) {
		if (!strcmp(*argv, "-recursive_backtracking")) {
			maze = maze_create_recursive_backtracking(50, 50, 5050);
		} else if (!strcmp(*argv, "-sidewinder")) {
			maze = maze_create_sidewinder(50, 50, 5050);
		} else if (!strcmp(*argv, "-binary_tree")) {
			maze = maze_create_binary_tree(50, 50, 5050);
		} else if (!strcmp(*argv, "-growing_tree")) {
			maze = maze_create_growing_tree(50, 50, 5050);
		} else if (!strcmp(*argv, "-kruskal")) {
			maze = maze_create_kruskal(50, 50, 5050);
		} else if (!strcmp(*argv, "-hunt_and_kill")) {
			maze = maze_create_hunt_and_kill(50, 50, 5050);
		} else {
			usage();
		}
	} else {
		usage();
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		dief("SDL_Init failed: %s", SDL_GetError());
	}

	window = SDL_CreateWindow("sdl2_maze_renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (NULL == window) {
		dief("SDL_CreateWindow failed: %s", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (NULL == renderer) {
		dief("SDL_CreateRenderer failed: %s", SDL_GetError());
	}

	while (1) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				goto quit;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_GetWindowSize(window, &width, &height);

		width -= MARGIN * 2;
		height -= MARGIN * 2;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		for (y = 0; y < maze->height; ++y) {
			for (x = 0; x < maze->width; ++x) {
				ax = x * (width / maze->width) + MARGIN + (width - (maze->width) * (width / maze->width)) / 2;
				ay = y * (height / maze->height) + MARGIN + (height - (maze->height) * (height / maze->height)) / 2;
				bx = ax + (width / maze->width);
				by = ay;
				cx = ax + (width / maze->width);
				cy = ay + (height / maze->height);
				dx = ax;
				dy = cy;
				if (maze->data[y*maze->width+x] & MAZE_DIRECTION_NORTH) SDL_RenderDrawLine(renderer, ax, ay, bx, by);
				if (maze->data[y*maze->width+x] & MAZE_DIRECTION_SOUTH) SDL_RenderDrawLine(renderer, dx, dy, cx, cy);
				if (maze->data[y*maze->width+x] & MAZE_DIRECTION_WEST) SDL_RenderDrawLine(renderer, ax, ay, dx, dy);
				if (maze->data[y*maze->width+x] & MAZE_DIRECTION_EAST) SDL_RenderDrawLine(renderer, bx, by, cx, cy);
			}
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 60);
	}

quit:
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}
