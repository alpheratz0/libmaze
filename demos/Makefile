.POSIX:
.PHONY: all clean

all: maze_png_renderer maze_sdl_renderer

libmaze.a:
	make -sC .. && mv ../libmaze.a . && make -sC .. clean

maze_png_renderer: maze-png-renderer.c maze-generic-renderer.h libmaze.a
	$(CC) $< libmaze.a -I.. -o $@ -lpng

maze_sdl_renderer: maze-sdl-renderer.c maze-generic-renderer.h libmaze.a
	$(CC) $< libmaze.a -I.. -o $@ -lSDL2

clean:
	rm -f libmaze.a maze_png_renderer maze_sdl_renderer
