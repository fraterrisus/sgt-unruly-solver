package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Board;
import com.hitchhikerprod.unruly.Square;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class ByPerms {
    private final Board board;

    public ByPerms(Board board) {
        this.board = board;
    }

    public boolean solve() {
        boolean updates = false;
        for (int y = 0; y < board.yDim; y++) {
            final int fy = y;
            final List<Coordinate> coords = IntStream.range(0, board.xDim)
                    .mapToObj(x -> new Coordinate(x, fy)).toList();
            updates |= doIt(coords);
        }
        for (int x = 0; x < board.xDim; x++) {
            final int fx = x;
            final List<Coordinate> coords = IntStream.range(0, board.xDim)
                    .mapToObj(y -> new Coordinate(fx, y)).toList();
            updates |= doIt(coords);
        }
        return updates;
    }

    private boolean doIt(List<Coordinate> coords) {
//        final Map<Square.Value, Long> partition = coords.stream().map(c -> board.get(c.x(), c.y()))
//                .collect(Collectors.groupingBy(Function.identity(), Collectors.counting()));
        final long half = coords.size() / 2;
        final long blacks = half; // - partition.getOrDefault(Square.Value.BLACK, 0L);
        final long whites = half; // - partition.getOrDefault(Square.Value.WHITE, 0L);

        final List<List<Square.Value>> possibles = new ArrayList<>();

        recurse(
                possibles,
                new ArrayList<>(),
                Math.toIntExact(blacks),
                Math.toIntExact(whites),
                coords,
                0
        );

        // we shouldn't be able to enumerate all conditions and come up with _zero_ options :(
        if (possibles.isEmpty()) return false;

        boolean anyMatch = false;
        for (int i = 0; i < coords.size(); i++) {
            final Coordinate c = coords.get(i);
            if (board.get(c.x(), c.y()) != Square.Value.CLEAR) continue;
            boolean allMatch = true;
            int pn = 1;
            Square.Value first = possibles.getFirst().get(i);
            while (pn < possibles.size()) {
                if (first != possibles.get(pn).get(i)) {
                    allMatch = false;
                    break;
                }
                pn++;
            }
            if (allMatch) {
                anyMatch = true;
//                System.out.format("Setting (%d,%d) = %s\n", c.x(), c.y(), first);
                board.set(c.x(), c.y(), first);
            }
        }

        return anyMatch;
    }

    private void recurse(
            final List<List<Square.Value>> possibles,
            final List<Square.Value> workingSet,
            final int blacksRemaining,
            final int whitesRemaining,
            final List<Coordinate> remaining,
            final int i
    ) {
//        System.out.format("%s[%d] B:%d W:%d\n", " ".repeat(i*2), i, blacksRemaining, whitesRemaining);

        if (i >= remaining.size()) {
//            System.out.print(" ".repeat(i*2));
/*
            System.out.println("End of stack: " + workingSet.stream()
                    .map(Square.Value::toChar)
                    .map(String::valueOf)
                    .collect(Collectors.joining(" ")));
*/
            possibles.add(new ArrayList<>(workingSet));
            return;
        }

        final Coordinate c = remaining.get(i);
        final Square.Value oldValue = board.get(c.x(), c.y());

        if (blacksRemaining > 0 && canBe(c, Square.Value.BLACK)) {
//            System.out.print(" ".repeat(i*2));
//            System.out.println("BLACK");
            workingSet.addLast(Square.Value.BLACK);
            board.set(c.x(), c.y(), Square.Value.BLACK);
            recurse(possibles, workingSet, blacksRemaining-1, whitesRemaining, remaining, i+1);
            board.set(c.x(), c.y(), oldValue);
            workingSet.removeLast();
        }
        if (whitesRemaining > 0 && canBe(c, Square.Value.WHITE)) {
//            System.out.print(" ".repeat(i*2));
//            System.out.println("WHITE");
            workingSet.addLast(Square.Value.WHITE);
            board.set(c.x(), c.y(), Square.Value.WHITE);
            recurse(possibles, workingSet, blacksRemaining, whitesRemaining-1, remaining, i+1);
            board.set(c.x(), c.y(), oldValue);
            workingSet.removeLast();
        }

//        System.out.print(" ".repeat(i*2));
//        System.out.println("return");
    }

    private boolean canBe(Coordinate c, Square.Value val) {
        final int x = c.x();
        final int y = c.y();

        final Square.Value v0 = board.get(c.x(), c.y());
        if (v0 == val) return true;
        if (v0 == val.invert()) return false;

        if (safeIs(x-2, y, val) && safeIs(x-1, y, val)) return false;
        if (safeIs(x-1, y, val) && safeIs(x+1, y, val)) return false;
        if (safeIs(x+1, y, val) && safeIs(x+2, y, val)) return false;

        if (safeIs(x, y-2, val) && safeIs(x, y-1, val)) return false;
        if (safeIs(x, y-1, val) && safeIs(x, y+1, val)) return false;
        if (safeIs(x, y+1, val) && safeIs(x, y+2, val)) return false;

        return true;
    }

    private boolean safeIs(int x, int y, Square.Value value) {
        final Optional<Square.Value> sq = safeGet(x, y);
        return sq.isPresent() && sq.get() == value;
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
