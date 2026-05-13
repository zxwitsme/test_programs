"""
Calculate Pi (圆周率) using multiple methods:
1. Leibniz formula
2. Nilakantha series
3. Monte Carlo simulation
4. Machin formula
"""

import random
import math
import time


def leibniz(iterations=1_000_000):
    """Leibniz formula: pi/4 = 1 - 1/3 + 1/5 - 1/7 + ..."""
    pi = 0.0
    for i in range(iterations):
        pi += ((-1) ** i) / (2 * i + 1)
    return pi * 4


def nilakantha(iterations=1000):
    """Nilakantha series: pi = 3 + 4/(2*3*4) - 4/(4*5*6) + ..."""
    pi = 3.0
    sign = 1
    for i in range(1, iterations + 1):
        n = 2 * i
        pi += sign * 4.0 / (n * (n + 1) * (n + 2))
        sign = -sign
    return pi


def monte_carlo(samples=1_000_000):
    """Monte Carlo: randomly sample points in a unit square, count those in the circle."""
    inside = 0
    for _ in range(samples):
        x = random.random()
        y = random.random()
        if x * x + y * y <= 1.0:
            inside += 1
    return 4.0 * inside / samples


def machin():
    """Machin formula: pi/4 = 4*arctan(1/5) - arctan(1/239)"""
    def arctan(x, terms=50):
        result = 0.0
        x_power = x
        x_sq = x * x
        for n in range(terms):
            result += ((-1) ** n) * x_power / (2 * n + 1)
            x_power *= x_sq
        return result

    return 4.0 * (4 * arctan(1/5) - arctan(1/239))


def main():
    print(f"math.pi (reference): {math.pi:.15f}\n")

    methods = [
        ("Leibniz (1,000,000 iterations)", lambda: leibniz(1_000_000)),
        ("Nilakantha (1,000 iterations)", lambda: nilakantha(1000)),
        ("Monte Carlo (1,000,000 samples)", lambda: monte_carlo(1_000_000)),
        ("Machin formula", machin),
    ]

    for name, func in methods:
        start = time.perf_counter()
        result = func()
        elapsed = time.perf_counter() - start
        error = abs(result - math.pi)
        print(f"{name}")
        print(f"  pi ≈ {result:.15f}")
        print(f"  error: {error:.2e}  time: {elapsed:.3f}s\n")


if __name__ == "__main__":
    main()
