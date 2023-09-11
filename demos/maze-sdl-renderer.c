// Copyright (C) 2023 <alpheratz99@protonmail.com>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "maze.h"
#include "algorithms.h"
#include "maze-generic-renderer.h"

#define WIN_WIDTH 400
#define WIN_HEIGHT 400
#define MAZE_ROWS 50
#define MAZE_COLUMS 50

#define USE_LINES

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
} MazeSdlRendererApp_t;

static void
usage(void)
{
	fprintf(stderr, "usage: maze_sdl_renderer [-recursive_backtracking] [-sidewinder]\n"
					"                         [-binary_tree] [-growing_tree] [-kruskal]\n"
					"                         [-hunt_and_kill]\n");
	exit(EXIT_FAILURE);
}

static void
draw_line(point_t p0, point_t p1, void *userdata)
{
	SDL_Renderer *renderer;

	renderer = userdata;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, p0.x, p0.y, p1.x, p1.y);
}

static void
draw_rect(rect_t rect, void *userdata)
{
	SDL_Renderer *renderer;

	renderer = userdata;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &(const SDL_Rect){
		.x = rect.x,
		.y = rect.y,
		.w = rect.w,
		.h = rect.h
	});
}

static void
init_sdl_app(MazeSdlRendererApp_t *app)
{
	uint32_t renderer_flags;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "couldn't init sdl: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	app->window = SDL_CreateWindow("maze sdl renderer",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

	if (NULL == app->window) {
		fprintf(stderr, "couldn't create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	app->renderer = SDL_CreateRenderer(app->window, -1, renderer_flags);

	if (NULL == app->renderer) {
		fprintf(stderr, "couldn't create renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

static bool
process_events(void)
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
		if (ev.type == SDL_QUIT)
			return true;
	return false;
}

static void
fini_sdl_app(MazeSdlRendererApp_t *app)
{
	SDL_DestroyWindow(app->window);
	SDL_DestroyRenderer(app->renderer);
	SDL_Quit();
}

static maze_algorithm_t
maze_algorithm_from_opt(const char *opt)
{
	if (!strcmp(opt, "-recursive_backtracking")) return maze_recursive_backtracking;
	if (!strcmp(opt, "-sidewinder")) return maze_sidewinder;
	if (!strcmp(opt, "-binary_tree")) return maze_binary_tree;
	if (!strcmp(opt, "-growing_tree")) return maze_growing_tree;
	if (!strcmp(opt, "-hunt_and_kill")) return maze_hunt_and_kill;
	return NULL;
}

int
main(int argc, char **argv)
{
	maze_t maze;
	maze_algorithm_t alg;
	MazeSdlRendererApp_t app;
	bool should_quit = false;

	alg = maze_kruskal;

	if (argc > 1 && (NULL == (alg = maze_algorithm_from_opt(argv[1]))))
		usage();

	srand(time(NULL));
	alg(&maze, MAZE_COLUMS, MAZE_ROWS, rand());

	init_sdl_app(&app);

	while (!should_quit) {
		should_quit = process_events();

		SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(app.renderer);

#ifdef USE_LINES
		maze_generic_render_lines(&maze, WIN_WIDTH, WIN_HEIGHT,
				draw_line, app.renderer);
#else
		maze_generic_render_boxes(&maze, WIN_WIDTH, WIN_HEIGHT,
				draw_rect, app.renderer);
#endif

		SDL_RenderPresent(app.renderer);
		SDL_Delay(1000 / 60);
	}

	fini_sdl_app(&app);

	return 0;
}
