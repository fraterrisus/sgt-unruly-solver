CC=g++
CXXFLAGS=-std=c++11 -ggdb

TARGETS=solver.o board.o board_iterator.o square.o

default: solver

board.o: board.h board.cpp square.o

board_iterator.o: board.h board_iterator.cpp square.o

solver.o: board.o board_iterator.o

square.o: square.h square.cpp

solver: $(TARGETS)

clean:
	rm $(TARGETS)
