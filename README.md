# sgt-unruly-solver
A solver for the game "unruly" from Simon Tatham's puzzle collection.

Runtimes (in ms, over 1000 runs so reported to 3 significant digits)

Board Size | Difficulty | With Heuristics | Brute Force Only
----- | ------ | ---- | ----
8x8   | Easy   | 1.78 | 4.53
8x8   | Normal | 3.37 | 5.43
10x10 | Easy   | 3.65 | 13.2
10x10 | Normal | 7.27 | 16.2
14x14 | Easy   | 10.5 | 88.1
14x14 | Normal | 23.4 | 125

If you're looking to replicate these experiments, you'll want to `#define` or
`#undef USE_HEURISTICS` in `board.cpp`. Also I've included `speedtest.sh` for
your convenience.
