#ifndef sudoku_h
#define sudoku_h

typedef enum
{
    SUDOKU_SOLVED,
    SUDOKU_UNSOLVED,
    SUDOKU_BADINPUT,
}
sudoku_errno;

/* Try to solve a sudoku board
 * Returns a sudoku_errno */
int solve_sudoku(int board[9][9]);

#endif
