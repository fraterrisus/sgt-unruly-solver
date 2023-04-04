package com.hitchhikerprod.unruly;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Board {
    private static final Pattern GAME_ID_REGEX = Pattern.compile("^(\\d+)x(\\d+)([a-z]?):([a-zA-Z]+)\\s*$");

    private final List<Square> squares;
    public final int xDim;
    public final int yDim;

    public Board(int xDim, int yDim) {
        this.xDim = xDim;
        this.yDim = yDim;
        this.squares = new ArrayList<>();
        for (int i = 0 ; i < xDim * yDim; i++) { squares.add(new Square()); }
    }

    public Board copy() {
        final Board newBoard = new Board(this.xDim, this.yDim);
        int i = 0;
        for (Square oldSquare : this.squares) {
            newBoard.squares.set(i, new Square(oldSquare.get()));
            i++;
        }
        return newBoard;
    }

    public boolean isSolved() {
        return this.squares.stream().allMatch(Square::isFilled);
    }

    public boolean isValid() {
        for (int y = 0; y < yDim; y++) {
            for (int x = 0; x < xDim; x++) {
                final Square.Value me = get(x,y);
                if (me == Square.Value.CLEAR) continue;
                try {
                    if ((me == get(x + 1, y)) && (me == (get(x + 2, y)))) return false;
                } catch (IndexOutOfBoundsException ignored) {}
                try {
                    if ((me == get(x, y + 1)) && (me == (get(x, y + 2)))) return false;
                } catch (IndexOutOfBoundsException ignored) {}
            }
        }
        return true;
    }

    private int coordinateToIndex(int x, int y) {
        final int index = (y * xDim) + x;
        if ((index < 0) || (index > (xDim * yDim))) {
            final String message = "Coordinate (" + x + "," + y + ") is out of bounds";
            throw new IndexOutOfBoundsException(message);
        }
        return index;
    }

    public void set(int x, int y, Square.Value value) {
        squares.get(coordinateToIndex(x, y)).set(value);
    }

    public Square.Value get(int x, int y) {
        return squares.get(coordinateToIndex(x, y)).get();
    }

    public List<Square> getRow(int y) {
        final List<Square> row = new ArrayList<>();
        int index = y * xDim;
        for (int i = 0; i < xDim; i++) {
            row.add(squares.get(index));
            index++;
        }
        return Collections.unmodifiableList(row);
    }

    public List<Square> getCol(int x) {
        final List<Square> col = new ArrayList<>();
        int index = x;
        for (int i = 0; i < yDim; i++) {
            col.add(squares.get(index));
            index += xDim;
        }
        return Collections.unmodifiableList(col);
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        int index = 0;
        for (Square s : squares) {
            sb.append(s.toChar()).append(' ');
            index++;
            if (index % xDim == 0) {
                sb.append("\n");
            }
        }
        return sb.toString();
    }

    public static Board fromGameId(String gameId) {
        final Matcher matcher = GAME_ID_REGEX.matcher(gameId);
        if (!matcher.matches()) {
            throw new RuntimeException("Game ID string couldn't be parsed");
        }

        final int xDim = Integer.parseInt(matcher.group(1));
        final int yDim = Integer.parseInt(matcher.group(2));
        final Board newBoard = new Board(xDim, yDim);

        // final String difficulty = matcher.group(3);
        final String squares = matcher.group(4);
        final int[] ords = squares.chars().toArray();
        int index = -1;
        Square.Value newColor = Square.Value.CLEAR;
        for (int o : ords) {
            if (o >= 65 && o <= 90) {
                index += o - 64;
                newColor = Square.Value.BLACK;
            } else if (o >= 97 && o <= 122) {
                index += o - 96;
                newColor = Square.Value.WHITE;
            }
            if (index >= xDim * yDim) { return newBoard; }
            newBoard.squares.get(index).set(newColor);
        }
        throw new RuntimeException("Game ID string didn't properly terminate");
    }
}
