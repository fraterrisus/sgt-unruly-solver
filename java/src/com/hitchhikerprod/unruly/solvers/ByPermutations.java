package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Board;
import com.hitchhikerprod.unruly.Square;

import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class ByPermutations implements BoardStrategy {
    @Override
    public Board solve(Board board) {
        for (int x = 0; x < board.xDim; x++) {
            final List<Square> col = board.getCol(x);
            final Board newBoard = board.copy();
            final boolean changed = solve(newBoard, col);
            if (changed) return newBoard;
        }
        for (int y = 0; y < board.yDim; y++) {
            final List<Square> row = board.getRow(y);
            final Board newBoard = board.copy();
            final boolean changed = solve(newBoard, row);
            if (changed) return newBoard;
        }
        return null;
    }

    private boolean solve(Board board, List<Square> squares) {
        final Map<Square.Value, Long> partition = squares.stream()
            .collect(Collectors.groupingBy(Square::get, Collectors.counting()));
        if (partition.getOrDefault(Square.Value.CLEAR, 0L) == 0L) { return false; }

        final List<Integer> clearIndices = new ArrayList<>();
        int index = 0;
        for (Square square : squares) {
            if (! square.isFilled()) {
                clearIndices.add(index);
                square.set(Square.Value.WHITE);
            }
            index++;
        }

        final long missingBlack = (squares.size() / 2) - partition.getOrDefault(Square.Value.BLACK, 0L);
        final long missingWhite = (squares.size() / 2) - partition.getOrDefault(Square.Value.WHITE, 0L);
        final long missing = missingBlack + missingWhite;

        /* Each permutation is a list of squares that should be set to BLACK. When you inspect a BitSet it shows you
         * the set of bits that are "on", i.e. {0,2} means 1010. We use "clearIndices" to map those indices to the
         * positions of unassigned squares in the row (which might be positions 0, 1, 6, and 8, for example). */
        final List<BitSet> perms = IntStream.range(0, (int) Math.pow(2, missing))
            .mapToObj(i -> new long[]{i})
            .map(BitSet::valueOf)
            .filter(bs -> bs.cardinality() == missingBlack)
            .toList();

        /* Run through each permutation and assign those squares to BLACK. Then ask the Board if it is "valid".
         * Any permutations that result in valid boards wind up in "validPerms". */
        final List<BitSet> validPerms = new ArrayList<>();
        for (BitSet bs : perms) {
            bs.stream().forEach(idx -> squares.get(clearIndices.get(idx)).set(Square.Value.BLACK));
            System.out.println(board);
            if (board.isValid()) validPerms.add(bs);
            clearIndices.forEach(idx -> squares.get(idx).set(Square.Value.WHITE));
        }

        /* Now we need to reduce the "validPerms" to only the bits that are the same in every valid permutation, i.e.
         * where all perms are WHITE or all perms are BLACK. Anything where two valid permutations differ is a square
         * that we *can't* assign yet. */
        final BitSet assignable = new BitSet((int)missing);
        for (BitSet vs : validPerms) {
            for (int i = 0; i < missing; i++) {
                if (vs.get(i) != validPerms.get(0).get(i)) assignable.set(i);
            }
        }
        assignable.flip(0,(int)missing);
        if (assignable.cardinality() == 0) return false;

        clearIndices.forEach(idx -> squares.get(idx).set(Square.Value.CLEAR));
        assignable.stream().forEach(idx -> {
            final Square sq = squares.get(clearIndices.get(idx));
            if (validPerms.get(0).get(idx)) sq.set(Square.Value.BLACK);
            else sq.set(Square.Value.WHITE);
        });
        return true;
    }
}
