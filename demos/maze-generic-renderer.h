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

#include "maze.h"

#define MARGIN 2

#define RECT(x,y,w,h) (const rect_t){x,y,w,h}
#define POINT(x,y) (const point_t){x,y}

typedef struct { int x, y; } point_t;
typedef struct { int x, y, w, h; } rect_t;

typedef void (*draw_line_t)(point_t p0, point_t p1, void *userdata);
typedef void (*draw_rect_t)(rect_t rect, void *userdata);

static void
maze_generic_render_lines(maze_t *m, int w, int h, draw_line_t draw_line, void *userdata)
{
	int x, y;
	int off_x, off_y;
	int cell_w, cell_h;
	point_t a, b, c, d;

	w = w - MARGIN * 2;
	h = h - MARGIN * 2;

	cell_w = w / m->width;
	cell_h = h / m->height;

	off_x = MARGIN + (w % m->width) / 2;
	off_y = MARGIN + (h % m->height) / 2;

	for (y = 0; y < m->height; ++y) {
		for (x = 0; x < m->width; ++x) {
			a = POINT(off_x + x * cell_w, off_y + y * cell_h);
			b = POINT(a.x + cell_w, a.y);
			c = POINT(a.x, a.y + cell_h);
			d = POINT(a.x + cell_w, a.y + cell_h);

			if (m->data[y*m->width+x] & MAZE_WALL_NORTH) draw_line(a, b, userdata);
			if (m->data[y*m->width+x] & MAZE_WALL_SOUTH) draw_line(c, d, userdata);
			if (m->data[y*m->width+x] & MAZE_WALL_WEST) draw_line(a, c, userdata);
			if (m->data[y*m->width+x] & MAZE_WALL_EAST) draw_line(b, d, userdata);
		}
	}
}

static void
maze_generic_render_boxes(maze_t *m, int w, int h, draw_rect_t draw_rect, void *userdata)
{
	int x, y;
	int off_x, off_y;
	int cell_w, cell_h;
	point_t a, b, c;

	w = w - MARGIN * 2;
	h = h - MARGIN * 2;

	cell_w = w / (m->width * 2 + 1);
	cell_h = h / (m->height * 2 + 1);

	off_x = MARGIN + (w % (m->width * 2 + 1)) / 2;
	off_y = MARGIN + (h % (m->height * 2 + 1)) / 2;

	for (y = 0; y < m->height; ++y) {
		for (x = 0; x < m->width; ++x) {
			a = POINT(off_x + x * 2 * cell_w, off_y + y * 2 * cell_h);
			b = POINT(a.x + cell_w * 2, a.y);
			c = POINT(a.x, a.y + cell_h * 2);

			if (m->data[y*m->width+x] & MAZE_WALL_NORTH)
				draw_rect(RECT(a.x, a.y, cell_w * 3, cell_h), userdata);

			if (m->data[y*m->width+x] & MAZE_WALL_SOUTH)
				draw_rect(RECT(c.x, c.y, cell_w * 3, cell_h), userdata);

			if (m->data[y*m->width+x] & MAZE_WALL_WEST)
				draw_rect(RECT(a.x, a.y, cell_w, cell_h * 3), userdata);

			if (m->data[y*m->width+x] & MAZE_WALL_EAST)
				draw_rect(RECT(b.x, b.y, cell_w, cell_h * 3), userdata);
		}
	}
}
