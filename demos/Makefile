.POSIX:
.PHONY: all clean

all: png_maze_renderer sdl2_maze_renderer

png_maze_renderer: png_maze_renderer.o
	cc png_maze_renderer.c ../*.c ../util/*.c -o png_maze_renderer -lpng

sdl2_maze_renderer: sdl2_maze_renderer.o
	cc sdl2_maze_renderer.c ../*.c ../util/*.c -o sdl2_maze_renderer -lSDL2

clean:
	rm -f png_maze_renderer sdl2_maze_renderer *.o
