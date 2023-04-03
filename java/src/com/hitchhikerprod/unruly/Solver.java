package com.hitchhikerprod.unruly;

import com.hitchhikerprod.unruly.solvers.*;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.time.temporal.TemporalUnit;
import java.util.Date;
import java.util.List;

public class Solver {

    private Board board;
    private final List<RowStrategy> rowStrategies;
    private final List<BoardStrategy> boardStrategies;

    public Solver(Board board) {
        this.board = board;
        this.rowStrategies = List.of(new ByGaps(), new ByPairs(), new ByHalves());
        this.boardStrategies = List.of(new ByPermutations());
    }

    public void solve() {
        //System.out.println(board);

        boolean updates = true;
        while (updates && ! board.isSolved()) {
            //System.out.println(board);
            updates = false;
            for (int x = 0; x < board.xDim; x++) {
                final List<Square> col = board.getCol(x);
                updates |= runRowSolvers(col);
            }
            for (int y = 0; y < board.yDim; y++) {
                final List<Square> row = board.getRow(y);
                updates |= runRowSolvers(row);
            }

            if (updates) continue;

            updates = runBoardSolvers();
        }

        //System.out.println(board);
    }

    private boolean runRowSolvers(List<Square> row) {
        boolean updates = false;
        for (RowStrategy strat: rowStrategies) {
            updates |= strat.solve(row);
        }
        return updates;
    }

    private boolean runBoardSolvers() {
        for (BoardStrategy strat: boardStrategies) {
            final Board newBoard = strat.solve(board);
            if (newBoard != null) {
                this.board = newBoard;
                return true;
            }
        }
        return false;
    }

    private static String getGameId(String size) {
        try {
            final ProcessBuilder pb = new ProcessBuilder("/usr/games/sgt-unruly", "--generate", "1", size);
            pb.redirectOutput(ProcessBuilder.Redirect.PIPE);
            final Process process = pb.start();
            final byte[] processOutput = process.getInputStream().readAllBytes();
            return new String(processOutput, StandardCharsets.UTF_8);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        final String gameId = getGameId("30x30");
        System.out.println(gameId);
        final Board board = Board.fromGameId(gameId);
        long solveTimeUS = 0L;
        int numTrials = 100;

        for (int i = 0; i < numTrials; i++) {
            final Solver solver = new Solver(board);
            final Instant startTime = Instant.now();
            solver.solve();
            final Instant endTime = Instant.now();
            solveTimeUS += startTime.until(endTime, ChronoUnit.MICROS);
        }

        System.out.printf("Solved in %.3f ms\n", (double)solveTimeUS / 1000.0);
    }
}
