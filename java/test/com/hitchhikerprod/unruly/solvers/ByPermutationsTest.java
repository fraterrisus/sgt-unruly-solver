package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Board;
import com.hitchhikerprod.unruly.Square;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class ByPermutationsTest {
    @Test
    public void testSimple1() {
        final Board input = new Board(4, 4);
        input.set(0, 0, Square.Value.BLACK);
        input.set(1, 2, Square.Value.BLACK);
        input.set(2, 2, Square.Value.BLACK);
        final ByPermutations uut = new ByPermutations();
        final Board newBoard = uut.solve(input);
        // System.out.println(newBoard);
    }
}