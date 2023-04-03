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

    public boolean isFilled() {
        return value != Value.CLEAR;
    }

    public Value inverted() {
        return this.value.invert();
    }

    public void invert() {
        this.value = this.value.invert();
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Square that) {
            return this.get() == that.get();
        } else {
            return false;
        }
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

    public String toString() {
        return this.value.toString();
    }
}
