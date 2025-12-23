package com.hitchhikerprod.unruly;

import com.hitchhikerprod.unruly.solvers.BoardStrategy;
import com.hitchhikerprod.unruly.solvers.ByClear;
import com.hitchhikerprod.unruly.solvers.ByGaps;
import com.hitchhikerprod.unruly.solvers.ByHalves;
import com.hitchhikerprod.unruly.solvers.ByPairs;
import com.hitchhikerprod.unruly.solvers.ByPerms;
import com.hitchhikerprod.unruly.solvers.ByPermutations;
import com.hitchhikerprod.unruly.solvers.RowStrategy;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.LinkedList;
import java.util.List;
import java.util.LongSummaryStatistics;
import java.util.stream.Collectors;

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

//        System.out.println(board);
    }

    public void solve2() {
        final ByClear byClear = new ByClear(board);
        final ByHalves byHalves = new ByHalves();
        final ByPerms byPerms = new ByPerms(board);

        boolean updates = true;
        while (updates) {
//            System.out.println(board);

            updates = byClear.solve();
            if (updates) continue;

            for (int x = 0; x < board.xDim; x++) {
                final List<Square> col = board.getCol(x);
                updates |= byHalves.solve(col);
            }
            for (int y = 0; y < board.yDim; y++) {
                final List<Square> row = board.getRow(y);
                updates |= byHalves.solve(row);
            }
            if (updates) continue;

            updates = byPerms.solve();
        }

//        System.out.println(board);
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

/*    public static void main(String[] argv) {
        final String gameId;
        try {
            gameId = new String(System.in.readAllBytes(), StandardCharsets.UTF_8);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        Instant startTime, endTime;
        Board board;
        Solver solver;

        startTime = Instant.now();
        board = Board.fromGameId(gameId);
        solver = new Solver(board);
        solver.solve();
        endTime = Instant.now();
        System.out.printf("Solve1 time: %.6f s\n", startTime.until(endTime, ChronoUnit.MICROS) / 1000000.0);

        startTime = Instant.now();
        board = Board.fromGameId(gameId);
        solver = new Solver(board);
        solver.solve2();
        endTime = Instant.now();
        System.out.printf("Solve2 time: %.6f s\n", startTime.until(endTime, ChronoUnit.MICROS) / 1000000.0);
    }*/

    public static void main(String[] args) {
        int numTrials = 1000;

        // Prime the pump (well, the JITter, really)
        for (int i = 0; i < numTrials; i++) {
            final String gameId = getGameId("10x10de");

            final Board board = Board.fromGameId(gameId);
            final Solver solver = new Solver(board);
            solver.solve2();
        }

        List<Long> solveTimes = new LinkedList<>();

        System.out.print("   Size      min       avg       max  (us)\n");
        for (String diff : List.of("10x10de", "10x10dn", "14x14de", "14x14dn", "20x20de", "20x20dn")) {
            solveTimes.clear();

            for (int i = 0; i < numTrials; i++) {
                final String gameId = getGameId(diff);

                final Instant startTime = Instant.now();

                final Board board = Board.fromGameId(gameId);
                final Solver solver = new Solver(board);
                solver.solve();

                final Instant endTime = Instant.now();
                solveTimes.add(startTime.until(endTime, ChronoUnit.MICROS));
            }

            final LongSummaryStatistics stats = solveTimes.stream().collect(Collectors.summarizingLong(x -> x));

            System.out.printf("%8s  %6d  %8.1f  %8d\n", diff, stats.getMin(), stats.getAverage(), stats.getMax());
        }
    }
}
