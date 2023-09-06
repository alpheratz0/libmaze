.POSIX:
.PHONY: all clean install

include config.mk

OBJ=\
	maze.o \
	sidewinder.o \
	util/dynarr.o \
	recursive-backtracking.o

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
