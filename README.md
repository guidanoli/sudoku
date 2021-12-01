# Sudoku Solver

Sudoku (数独, sūdoku, digit-single) is a logic-based, combinatorial number-placement puzzle. In classic sudoku, the objective is to fill a 9×9 grid with digits so that each column, each row, and each of the nine 3×3 subgrids that compose the grid (also called "boxes", "blocks", or "regions") contain all of the digits from 1 to 9. The puzzle setter provides a partially completed grid, which for a well-posed puzzle has a single solution. (Wikipedia)

## Solving a sudoku board

```
Usage: sudokusolver [<input-file> [<output-file>]]

If <input-file> is missing, reads problem from stdin
If <output-file> is missing, writes solution to stdout
```

## Generating a random sudoku board

Keep in mind that not all sudoku boards are solvable.

```
Usage: randomsudoku [options]

Options:
  -h             Displays this message and exit immediately
  -c[MIN-CELLS]  Set minimum number of filled cells
  -C[MAX-CELLS]  Set maxmimum number of filled cells
  -n[BOARD-CNT]  Number of boards to be generated
```

## Solving random sudoku boards

Generates 10 random input boards and save them to boards.txt.
The output will be the solution of all the solvable boards.

```sh
randomsudoku -n10 | tee boards.txt | sudokusolver
```

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
