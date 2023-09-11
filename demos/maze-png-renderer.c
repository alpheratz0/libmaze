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

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <png.h>

#include "maze-generic-renderer.h"
#include "algorithms.h"
#include "maze.h"

typedef struct {
	uint32_t *px;
	int w;
	int h;
} Image_t;

static void
die(const char *fmt, ...)
{
	va_list args;

	fputs("maze_png_renderer: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
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
usage(void)
{
	fprintf(stderr, "usage: maze_png_renderer [-recursive_backtracking] [-sidewinder] [-binary_tree] [-growing_tree]\n"
					"                         [-kruskal] [-hunt_and_kill] [-width w] [-height h] [-rows r] [-columns c]\n"
					"                         [-seed seed] [-lines] [-boxes]\n");
	exit(1);
}

static void
image_fill(Image_t *img, uint32_t c)
{
	size_t i;
	for (i = 0; i < img->w * img->h; ++i)
		img->px[i] = c;
}

static void
draw_line(point_t p0, point_t p1, void *userdata)
{
	int xy;
	Image_t *img;

	img = userdata;

	// vertical
	if (p0.x == p1.x) {
		for (xy = p0.y; xy <= p1.y; ++xy)
			img->px[xy*img->w+p0.x] = 0;
	} else {
		for (xy = p0.x; xy <= p1.x; ++xy)
			img->px[p0.y*img->w+xy] = 0;
	}
}

static void
draw_rect(rect_t rect, void *userdata)
{
	int x, y;
	Image_t *img;

	img = userdata;

	for (y = rect.y; y < rect.y + rect.h; ++y)
		for (x = rect.x; x < rect.x + rect.w; ++x)
			img->px[y*img->w+x] = 0;
}

static void
save_image_as_png(const char *path, Image_t *img)
{
	int x, y;
	FILE *fp;
	png_struct *png;
	png_info *pnginfo;
	png_byte *row;

	if (NULL == (fp = fopen(path, "wb")))
		die("fopen failed: %s", strerror(errno));

	if (NULL == (png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
		die("png_create_write_struct failed");

	if (NULL == (pnginfo = png_create_info_struct(png)))
		die("png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png)) != 0)
		die("aborting due to libpng error");

	png_init_io(png, fp);

	png_set_IHDR(
		png, pnginfo, img->w, img->h, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
	);

	png_write_info(png, pnginfo);
	png_set_compression_level(png, 3);

	row = malloc(img->w * 3);

	for (y = 0; y < img->h; ++y) {
		for (x = 0; x < img->w; ++x) {
			row[x*3+0] = (img->px[y*img->w+x] >> 16) & 0xff;
			row[x*3+1] = (img->px[y*img->w+x] >>  8) & 0xff;
			row[x*3+2] = (img->px[y*img->w+x] >>  0) & 0xff;
		}
		png_write_row(png, row);
	}

	png_write_end(png, NULL);
	png_free_data(png, pnginfo, PNG_FREE_ALL, -1);
	png_destroy_info_struct(png, &pnginfo);
	png_destroy_write_struct(&png, NULL);
	fclose(fp);
	free(row);
}

int
main(int argc, char **argv)
{
	int x, y;
	int rows, columns;
	int seed;
	Image_t img;
	char filename[256];
	maze_t maze = { 0 };
	maze_algorithm_t alg;
	bool lines;

	rows = 50;
	columns = 50;
	seed = 5050;
	img.w = 1024;
	img.h = 1024;
	lines = true;
	alg = maze_recursive_backtracking;

	while (++argv, --argc > 0) {
		if (!strcmp(*argv, "-recursive_backtracking")) {
			alg = maze_recursive_backtracking;
		} else if (!strcmp(*argv, "-sidewinder")) {
			alg = maze_sidewinder;
		} else if (!strcmp(*argv, "-binary_tree")) {
			alg = maze_binary_tree;
		} else if (!strcmp(*argv, "-growing_tree")) {
			alg = maze_growing_tree;
		} else if (!strcmp(*argv, "-kruskal")) {
			alg = maze_kruskal;
		} else if (!strcmp(*argv, "-hunt_and_kill")) {
			alg = maze_hunt_and_kill;
		} else if (!strcmp(*argv, "-width")) {
			--argc; img.w = atoi(enotnull(*++argv, "width"));
		} else if (!strcmp(*argv, "-height")) {
			--argc; img.h = atoi(enotnull(*++argv, "height"));
		} else if (!strcmp(*argv, "-rows")) {
			--argc; rows = atoi(enotnull(*++argv, "rows"));
		} else if (!strcmp(*argv, "-columns")) {
			--argc; columns = atoi(enotnull(*++argv, "columns"));
		} else if (!strcmp(*argv, "-seed")) {
			--argc; seed = atoi(enotnull(*++argv, "seed"));
		} else if (!strcmp(*argv, "-lines")) {
			lines = true;
		} else if (!strcmp(*argv, "-boxes")) {
			lines = false;
		} else {
			usage();
		}
	}

	alg(&maze, rows, columns, seed);

	img.px = malloc(sizeof(uint32_t) * img.w * img.h);

	image_fill(&img, 0xffffff);

	if (lines) {
		maze_generic_render_lines(&maze, img.w, img.h,
				draw_line, &img);
	} else {
		maze_generic_render_boxes(&maze, img.w, img.h,
				draw_rect, &img);
	}

	snprintf(filename, sizeof(filename), "%s_%dx%d_%d.png",
			maze.name, maze.width, maze.height, maze.seed);
	save_image_as_png(filename, &img);
	free(img.px);
	maze_fini(&maze);

	return 0;
}
