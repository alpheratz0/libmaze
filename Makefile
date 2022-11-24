.POSIX:
.PHONY: all clean install

include config.mk

all: libmaze.a

maze.o: maze.c maze.h

libmaze.a: maze.o
	$(AR) -rcs libmaze.a maze.o

install: libmaze.a
	rm -rf $(PREFIX)/include/maze
	mkdir $(PREFIX)/include/maze
	cp maze.h $(PREFIX)/include/maze
	cp libmaze.a $(PREFIX)/lib

clean:
	rm -f maze.o libmaze.a
