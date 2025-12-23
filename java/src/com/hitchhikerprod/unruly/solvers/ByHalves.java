package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Square;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class ByHalves implements RowStrategy {
    public boolean solve(List<Square> row) {
        boolean updates = false;
        final Map<Square.Value, List<Square>> partition = row.stream().collect(Collectors.groupingBy(Square::get));
        final List<Square> clears = partition.getOrDefault(Square.Value.CLEAR, List.of());
        if (clears.isEmpty()) return false;
        if (partition.getOrDefault(Square.Value.BLACK, List.of()).size() == row.size() / 2) {
            updates = ! clears.isEmpty();
            clears.forEach(s -> s.set(Square.Value.WHITE));
        } else if (partition.getOrDefault(Square.Value.WHITE, List.of()).size() == row.size() / 2) {
            updates = ! clears.isEmpty();
            clears.forEach(s -> s.set(Square.Value.BLACK));
        }
        return updates;
    }
}
