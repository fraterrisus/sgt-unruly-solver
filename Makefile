CC=g++

TARGETS=solver.o board.o square.o

default: solver

board.o: square.o

solver: $(TARGETS)

clean:
	rm $(TARGETS)
