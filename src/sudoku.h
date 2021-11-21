#ifndef sudoku_h
#define sudoku_h

enum sudoku_errno
{
    SUDOKU_OK, /* no problem */
    SUDOKU_INVALID_CELL, /* a cell is not between 0 and 9 */
    SUDOKU_INVALID_3X3, /* a cell is repeated in a 3x3 group */
    SUDOKU_INVALID_LINE, /* a cell is repeated in a 3x3 group */
};

struct sudoku_err
{
    int cell_i;
    int cell_j;
};

/* Try to solve a sudoku board
 * Returns a sudoku_errno enum */
enum sudoku_errno solve_sudoku(int board[9][9], struct sudoku_err* err);

#endif
