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

## Test

```sh
cmake --build build --target test
```
