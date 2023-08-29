.POSIX:
.PHONY: all clean install

include config.mk

OBJ=\
	binary_tree.o \
	growing_tree.o \
	hunt_and_kill.o \
	kruskal.o \
	ll.o \
	maze.o \
	recursive_backtracking.o \
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
