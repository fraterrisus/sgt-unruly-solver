package com.hitchhikerprod.unruly.solvers;

import com.hitchhikerprod.unruly.Square;

import java.util.List;

public interface RowStrategy {
    boolean solve(List<Square> squares);
}
