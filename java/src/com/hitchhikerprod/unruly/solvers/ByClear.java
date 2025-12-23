package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Board;
import com.hitchhikerprod.unruly.Square;

import java.util.LinkedHashSet;
import java.util.Optional;

public class ByClear {
    private final Board board;

    public ByClear(Board board) {
        this.board = board;
    }

    public boolean solve() {
        final LinkedHashSet<Coordinate> worklist = new LinkedHashSet<>();
        for (int y = 0; y < board.yDim; y++) {
            for (int x = 0; x < board.xDim; x++) {
                worklist.add(new Coordinate(x, y));
            }
        }

        boolean updates = false;
        while (!worklist.isEmpty()) {
            final Coordinate c = worklist.removeFirst();
            final int x = c.x();
            final int y = c.y();

            final Optional<Square.Value> me = safeGet(x, y);
            if (me.isEmpty() || (me.get() != Square.Value.CLEAR)) continue;

//            System.out.format("Checking (%d,%d)\n", x, y);

            final Optional<Square.Value> ll = safeGet(x-2, y);
            final Optional<Square.Value> l = safeGet(x-1, y);
            final Optional<Square.Value> r = safeGet(x+1, y);
            final Optional<Square.Value> rr = safeGet(x+2, y);

            final Optional<Square.Value> uu = safeGet(x, y-2);
            final Optional<Square.Value> u = safeGet(x, y-1);
            final Optional<Square.Value> d = safeGet(x, y+1);
            final Optional<Square.Value> dd = safeGet(x, y+2);

            if (ll.isPresent() && l.isPresent() && (l.get() != Square.Value.CLEAR) && (ll.get() == l.get())) {
                board.set(x, y, l.get().invert());
                updates = true;
                worklist.add(new Coordinate(x-3, y));
                worklist.add(new Coordinate(x+1, y));
                worklist.add(new Coordinate(x, y-1));
                worklist.add(new Coordinate(x, y+1));
            } else if (rr.isPresent() && r.isPresent() && (r.get() != Square.Value.CLEAR) && (rr.get() == r.get())) {
                board.set(x, y, r.get().invert());
                updates = true;
                worklist.add(new Coordinate(x-1, y));
                worklist.add(new Coordinate(x+3, y));
                worklist.add(new Coordinate(x, y-1));
                worklist.add(new Coordinate(x, y+1));
            } else if (uu.isPresent() && u.isPresent() && (u.get() != Square.Value.CLEAR) && (uu.get() == u.get())) {
                board.set(x, y, u.get().invert());
                updates = true;
                worklist.add(new Coordinate(x, y-3));
                worklist.add(new Coordinate(x, y+1));
                worklist.add(new Coordinate(x-1, y));
                worklist.add(new Coordinate(x+1, y));
            } else if (dd.isPresent() && d.isPresent() && (d.get() != Square.Value.CLEAR) && (dd.get() == d.get())) {
                board.set(x, y, d.get().invert());
                updates = true;
                worklist.add(new Coordinate(x, y-1));
                worklist.add(new Coordinate(x, y+3));
                worklist.add(new Coordinate(x-1, y));
                worklist.add(new Coordinate(x+1, y));
            } else if (l.isPresent() && r.isPresent() && (l.get() != Square.Value.CLEAR) && (l.get() == r.get())) {
                board.set(x, y, l.get().invert());
                updates = true;
                worklist.add(new Coordinate(x, y-1));
                worklist.add(new Coordinate(x, y+1));
            } else if (u.isPresent() && d.isPresent() && (u.get() != Square.Value.CLEAR) && (u.get() == d.get())) {
                board.set(x, y, u.get().invert());
                updates = true;
                worklist.add(new Coordinate(x-1, y));
                worklist.add(new Coordinate(x+1, y));
            }
        }

        return updates;
    }

    private Optional<Square.Value> safeGet(int x, int y) {
        if (x < 0) return Optional.empty();
        if (y < 0) return Optional.empty();
        if (x >= board.xDim) return Optional.empty();
        if (y >= board.yDim) return Optional.empty();
        final Square.Value value = board.get(x, y);
        return Optional.of(value);
    }
}
