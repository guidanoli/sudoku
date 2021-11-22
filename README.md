# Sudoku Solver

This is a limited sudoku solver.
It can solve any **easy** sudoku game.
A game of sudoku is said to be easy if at any given moment during your resolution of the game (accepting only new legal moves), there is always a cell with only one possible value to be filled or the game is finished.

## Dependencies

* CMake >= 3.16.3
* Any decent C99 compiler (only tested with gcc 9.3.0)

## Setup

```sh
cmake -B build
```

## Build

```sh
cmake --build build
```

## Usage

```sh
$ cat tests/board.txt
8 2 7   1 5 4   3 9 6
9 6 5   3 2 7   1 4 8
3 4 1   6 8 9   7 5 2

0 0 0   4 6 8   2 7 1
0 0 0   5 1 3   6 8 9
0 0 0   9 7 2   4 3 5

7 8 6   2 3 5   9 1 4
1 5 4   7 9 6   8 2 3
2 3 9   8 4 1   5 6 7

$ build/src/sudokusolver tests/board.txt
Solving board #1... Ok
Solution for board #1

8 2 7  1 5 4  3 9 6
9 6 5  3 2 7  1 4 8
3 4 1  6 8 9  7 5 2

5 9 3  4 6 8  2 7 1
4 7 2  5 1 3  6 8 9
6 1 8  9 7 2  4 3 5

7 8 6  2 3 5  9 1 4
1 5 4  7 9 6  8 2 3
2 3 9  8 4 1  5 6 7
```

## Test

```sh
cmake --build build --target test
```
