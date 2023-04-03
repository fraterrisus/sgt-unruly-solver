package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Solver;
import com.hitchhikerprod.unruly.Square;
import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

class ByHalvesTest {
    @Test
    public void testSolveHalves() {
        final List<Square> input = List.of(
            new Square(Square.Value.WHITE),
            new Square(Square.Value.WHITE),
            new Square(Square.Value.WHITE),
            new Square(Square.Value.WHITE),
            new Square(Square.Value.BLACK),
            new Square(Square.Value.BLACK),
            new Square(Square.Value.CLEAR),
            new Square(Square.Value.CLEAR)
        );

        final ByHalves uut = new ByHalves();
        uut.solve(input);
        assertEquals(input.get(6).get(), Square.Value.BLACK);
        assertEquals(input.get(7).get(), Square.Value.BLACK);

        final Map<Square.Value, Long> partition = input.stream()
            .collect(Collectors.groupingBy(Square::get, Collectors.counting()));
        assertEquals(partition.get(Square.Value.BLACK), 4);
        assertEquals(partition.get(Square.Value.WHITE), 4);
    }
}