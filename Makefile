CC=g++
CXXFLAGS=-std=c++11 -ggdb

TARGETS=solver.o board.o board_iterator.o square.o

default: solver

board.o: square.h

board_iterator.o: square.h

solver.o: board.h

solver: $(TARGETS)

clean:
	rm $(TARGETS)
