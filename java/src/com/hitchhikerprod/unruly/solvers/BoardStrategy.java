package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Board;

public interface BoardStrategy {
    /** Attempts to solve the board, probably speculatively, and therefore returns a Board object representing the
     * results of its strategy. This object may or may not be the same as the object that was passed in, but if the
     * caller wants to use the results of this solver it should accept the return value as the current Board.
     */
    Board solve(Board board);
}
