package com.hitchhikerprod.unruly;

public class Square {
    public enum Value {
        WHITE,
        BLACK,
        CLEAR;

        public Value invert() {
            switch (this) {
                case WHITE -> {
                    return BLACK;
                }
                case BLACK -> {
                    return WHITE;
                }
            }
            return null;
        }
    }

    private Value value;

    public Square() {
        this.value = Value.CLEAR;
    }

    public Square(Value value) {
        this.value = value;
    }

    public Value get() {
        return this.value;
    }

    public void set(Value newValue) {
        this.value = newValue;
    }

    public char toChar() {
        switch(this.value) {
            case BLACK -> {
                return 'x';
            }
            case WHITE -> {
                return 'o';
            }
            case CLEAR -> {
                return '.';
            }
        }
        return ' ';
    }
}
