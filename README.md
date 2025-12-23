# sgt-unruly-solver
A solver for the game `unruly` from [Simon Tatham's puzzle collection.](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/)

Results are in ms and are collected over 1,000 runs (of different puzzles), so they're reported out to 3 significant figures.

Last updated 2025-12-23.

### C++

| Board Size | Difficulty |  Avg |
|------------|------------|-----:|
| 10x10      | Easy       | 2.24 |
| 10x10      | Normal     | 4.27 |
| 14x14      | Easy       | 6.30 |
| 14x14      | Normal     | 13.4 |
| 20x20      | Easy       | 18.6 |
| 20x20      | Normal     | 44.1 |

### Java

In order to allow the JITter to do its work, my speed-test function runs 1,000 loop iterations at each problem size 
that shell out to `sgt-unruly` to generate a puzzle, parse the board, create a new `Solver` object, and run it to 
completion. (The time to generate the puzzle is not counted.) In order to allow the JITter to cache the bytecode, I 
also run 1,000 iterations but don't capture the runtime before I start the actual time trial. I think this reflects 
a more accurate sense of how fast the solver is.

| Board Size | Difficulty | Old solver | New solver |
|------------|------------|-----------:|-----------:|
| 10x10      | Easy       |      0.384 |      0.159 |
| 10x10      | Normal     |      0.502 |      0.303 |
| 14x14      | Easy       |      0.700 |      0.377 |
| 14x14      | Normal     |       1.53 |      0.779 |
| 20x20      | Easy       |       1.63 |      0.986 |
| 20x20      | Normal     |       17.4 |       2.77 |

It is kind of cool to see how the Easy puzzles can be solved entirely with heuristics, but when the brute-force solver
kicks in on the Hard puzzles, the solve time jumps. The new solver is more memory-optimized in that it doesn't 
speculatively create and destroy tons of new Board objects, it makes use of worklists instead of starting over with new
iterations, and the permutation solver uses recursion to generate options (which allows it to do more 
branch-and-bound work, instead of generating everything, filtering down, and then testing). 

### Ruby

TBD.