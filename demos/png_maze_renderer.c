/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////
/////  compile with:
/////
/////  cc png_maze_renderer.c ../*.c -o png_maze_renderer -lpng
/////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include "../maze.h"
#include "../algorithms.h"

#define MARGIN            20

typedef struct {
	int x;
	int y;
} Point;

static void
die(const char *fmt, ...)
{
	va_list args;

	fputs("png_maze_renderer: ", stderr);
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
	fprintf(stderr, "usage: png_maze_renderer [-recursive_backtracking] [-sidewinder] [-binary_tree] [-growing_tree]\n"
	                "                         [-kruskal] [-hunt_and_kill] [-width w] [-height h] [-rows r] [-columns c]\n"
	                "                         [-seed seed]\n");
	exit(1);
}

static void
image_fill(uint32_t *px, int w, int h, uint32_t c)
{
	size_t i;
	for (i = 0; i < w * h; ++i)
		px[i] = c;
}

static void
image_vh_line(uint32_t *px, int w, int h, Point from, Point to, uint32_t c)
{
	int xy;
	// vertical
	if (from.x == to.x) {
		for (xy = from.y; xy <= to.y; ++xy)
			px[xy*w+from.x] = c;
	} else {
		for (xy = from.x; xy <= to.x; ++xy)
			px[from.y*w+xy] = c;
	}
}

static void
save_buffer_as_png(const char *path, uint32_t *buffer, int width, int height)
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
		png, pnginfo, width, height, 8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE
	);

	png_write_info(png, pnginfo);
	png_set_compression_level(png, 3);

	row = malloc(width * 3);

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			row[x*3+0] = (buffer[y*width+x] >> 16) & 0xff;
			row[x*3+1] = (buffer[y*width+x] >>  8) & 0xff;
			row[x*3+2] = (buffer[y*width+x] >>  0) & 0xff;
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

static void
rect_to_points(int x, int y, int w, int h, Point *points)
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
	int x, y;
	int rows, columns;
	int seed;
	int img_width, img_height;
	int width, height;
	int cell_width, cell_height;
	int used_width, used_height;
	int unused_width, unused_height;
	uint32_t *pixels;
	Point pts[4];
	char filename[256];
	maze_t maze = { 0 };
	maze_algorithm_t alg;

	rows = 50;
	columns = 50;
	seed = 5050;
	img_width = 1024;
	img_height = 1024;
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
		} else if (!strcmp(*argv, "-width")) {
			--argc; img_width = atoi(enotnull(*++argv, "width"));
		} else if (!strcmp(*argv, "-height")) {
			--argc; img_height = atoi(enotnull(*++argv, "height"));
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

	alg(&maze, rows, columns, seed);

	pixels         =  malloc(sizeof(uint32_t) * img_width * img_height);
	width          =  img_width - MARGIN * 2;
	height         =  img_height - MARGIN * 2;
	cell_width     =  width / maze.width;
	cell_height    =  height / maze.height;
	used_width     =  cell_width * maze.width;
	used_height    =  cell_height * maze.height;
	unused_width   =  width - used_width;
	unused_height  =  height - used_height;

	image_fill(pixels, img_width, img_height, 0xffffff);

	for (y = 0; y < maze.height; ++y) {
		for (x = 0; x < maze.width; ++x) {
			rect_to_points(MARGIN + (x * cell_width) + unused_width / 2,
						   MARGIN + (y * cell_height) + unused_height / 2,
						   cell_width, cell_height, pts);

			if (maze.data[y*maze.width+x] & MAZE_WALL_NORTH)
				image_vh_line(pixels, img_width, img_height,
						pts[0], pts[1], 0x000000);

			if (maze.data[y*maze.width+x] & MAZE_WALL_SOUTH)
				image_vh_line(pixels, img_width, img_height,
						pts[3], pts[2], 0x000000);

			if (maze.data[y*maze.width+x] & MAZE_WALL_WEST)
				image_vh_line(pixels, img_width, img_height,
						pts[0], pts[3], 0x000000);

			if (maze.data[y*maze.width+x] & MAZE_WALL_EAST)
				image_vh_line(pixels, img_width, img_height,
						pts[1], pts[2], 0x000000);
		}
	}

	snprintf(filename, sizeof(filename), "%s_%dx%d_%d.png",
			maze.name, maze.width, maze.height, maze.seed);
	save_buffer_as_png(filename, pixels, img_width, img_height);
	free(pixels);
	maze_fini(&maze);

	return 0;
}
