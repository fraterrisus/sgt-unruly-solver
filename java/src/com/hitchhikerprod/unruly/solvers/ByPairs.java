package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Square;

import java.util.List;

public class ByPairs implements RowStrategy {
    @Override
    public boolean solve(List<Square> squares) {
        boolean updates = false;
        for (int i = 0; i < squares.size(); i++) {
            if (squares.get(i).get() != Square.Value.CLEAR) { continue; }

            try {
                final Square a = squares.get(i-2);
                final Square b = squares.get(i-1);
                if (a.isFilled() && a.equals(b)) {
                    squares.get(i).set(a.inverted());
                    updates = true;
                }
            } catch (IndexOutOfBoundsException ignored) { }

            try {
                final Square a = squares.get(i+2);
                final Square b = squares.get(i+1);
                if (a.isFilled() && a.equals(b)) {
                    squares.get(i).set(a.inverted());
                    updates = true;
                }
            } catch (IndexOutOfBoundsException ignored) { }
        }
        return updates;
    }
}
