# sgt-unruly-solver
A solver for the game `unruly` from [Simon Tatham's puzzle collection.](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/)

Results are in ms and are collected over 1,000 runs (of different puzzles), so they're reported out to 3 significant figures.

### C++

| Board Size | Difficulty |  Avg |
|------------|------------|-----:|
| 8x8        | Easy       | 1.08 |
| 8x8        | Normal     | 1.98 |
| 10x10      | Easy       | 2.17 |
| 10x10      | Normal     | 4.46 |
| 14x14      | Easy       | 6.71 |
| 14x14      | Normal     | 13.8 |

### Java

| Board Size | Difficulty | Avg (with JIT) | Avg (one run) |
|------------|------------|---------------:|--------------:|
| 8x8        | Easy       |          0.349 |          22.8 |
| 8x8        | Normal     |          0.374 |          33.6 |
| 10x10      | Easy       |          0.369 |          25.7 |
| 10x10      | Normal     |          0.406 |          37.2 |
| 14x14      | Easy       |          0.508 |          31.3 |
| 14x14      | Normal     |          0.822 |          46.8 |

On a hunch, I tried this solver two ways. The `with JIT` column reflects a `Main` class that runs a single process 
that shells out to `sgt-unruly` to generate a puzzle, parses the board, and creates a new `Solver` object 1,000 times.
(The time to generate the puzzle is not counted.) This allows the JITter to cache the bytecode, which I think reflects
a more accurate sense of how fast the solver is.

The `one run` column, on the other hand, works the same way as the C++ solver: a shell script calls `sgt-unruly` and
pipes the output into the solver (in this case, `java -jar ./solver.jar`). You wouldn't expect the JITter to have
nearly as much impact, and in fact the times here are _orders of magnitude_ worse. I validated this by looking at the 
`max` value collected during the `with JIT` run, and got similar results – and it's always the _first_ run that's the
slowest.

It is kind of cool to see how the Easy puzzles can be solved entirely with heuristics, but when the brute-force solver
kicks in on the Hard puzzles, the solve time jumps. There's probably some optimization that could be done there,
especially in terms of memory (the speculative `Board` creation is pretty foul).

### Ruby

TBD.