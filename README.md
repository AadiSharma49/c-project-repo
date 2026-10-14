# Matrix Calculator

A simple menu-based matrix calculator in C (college project).

## Features

- Addition
- Subtraction
- Multiplication
- Transpose
- Determinant

Matrices can be up to 10 x 10 and hold integers.

## How it works

The program shows a menu. You pick an operation, enter the size and
elements of the matrix (or matrices), and it prints the result.
It keeps showing the menu until you choose Exit.

## Rules

- Addition / Subtraction: both matrices must be the same size.
- Multiplication: columns of A must be equal to rows of B.
- Determinant: matrix must be square.

## Functions

- `readMatrix` - takes matrix elements from the user
- `printMatrix` - prints a matrix
- `readSize` - reads rows and columns (1 to 10)
- `add`, `subtract`, `multiply`, `transpose` - the operations
- `determinant` - recursive, expands along the first row

## Concepts used

2D arrays, functions, loops, recursion, if/else, switch-style menu.

## Files

- `src/main.c` - the whole program
