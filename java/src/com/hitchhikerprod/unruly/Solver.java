package com.hitchhikerprod.unruly;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class Solver {

    private static String getGameId() {
        try {
            final ProcessBuilder pb = new ProcessBuilder("/usr/games/sgt-unruly", "--generate", "1");
            pb.redirectOutput(ProcessBuilder.Redirect.PIPE);
            final Process process = pb.start();
            final byte[] processOutput = process.getInputStream().readAllBytes();
            return new String(processOutput, StandardCharsets.UTF_8);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        final String gameId = getGameId();
        System.out.println(gameId);
        final Board board = Board.fromGameId(gameId);
        System.out.print(board);
    }
}
