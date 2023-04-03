package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Square;

import java.util.List;

public class ByGaps implements RowStrategy {
    @Override
    public boolean solve(List<Square> squares) {
        boolean updates = false;
        for (int i = 1; i < squares.size()-1; i++) {
            if (squares.get(i).get() != Square.Value.CLEAR) { continue; }

            final Square a = squares.get(i-1);
            final Square b = squares.get(i+1);
            if (a.isFilled() && a.equals(b)) {
                squares.get(i).set(a.inverted());
                updates = true;
            }
        }
        return updates;
    }
}
