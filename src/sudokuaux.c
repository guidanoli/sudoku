#include "sudokuaux.h"
#include "sudoku.h"

const char* statusdescriptions[] = {
    [SUDOKU_OK] = "Ok",
    [SUDOKU_INVALID_CELL] = "Invalid cell",
    [SUDOKU_INVALID_3X3] = "Invalid 3x3",
    [SUDOKU_INVALID_LINE] = "Invalid line",
    [SUDOKU_INVALID_COLUMN] = "Invalid column",
    [SUDOKU_UNSOLVABLE] = "Unsolvable",
};

