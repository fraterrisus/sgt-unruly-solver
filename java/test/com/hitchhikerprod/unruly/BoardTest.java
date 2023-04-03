package com.hitchhikerprod.unruly;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class BoardTest {
    @Test
    public void testCopy() throws CloneNotSupportedException {
        final Board oldBoard = new Board(8, 8);
        oldBoard.set(5, 3, Square.Value.WHITE);
        final Board newBoard = oldBoard.copy();
        assertEquals(Square.Value.WHITE, newBoard.get(5, 3));
    }
}