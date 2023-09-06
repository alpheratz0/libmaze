.POSIX:
.PHONY: all clean install

include config.mk

OBJ=\
	util/dynarr.o \
	util/ll.o \
	maze.o \
	binary-tree.o \
	growing-tree.o \
	kruskal.o \
	recursive-backtracking.o \
	sidewinder.o

all: libmaze.a

libmaze.a: $(OBJ)
	$(AR) -rcs libmaze.a $(OBJ)

install: libmaze.a
	rm -rf $(PREFIX)/include/maze
	mkdir $(PREFIX)/include/maze
	cp *.h $(PREFIX)/include/maze
	cp libmaze.a $(PREFIX)/lib

clean:
	rm -f $(OBJ) libmaze.a
