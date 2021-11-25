#ifndef sudoku_h
#define sudoku_h

#include <stdint.h>

enum sudoku_errno
{
    SUDOKU_OK, /* board was solved in-place */
    SUDOKU_INVALID_CELL, /* a cell is not between 0 and 9 */
    SUDOKU_INVALID_3X3, /* a cell is repeated in a 3x3 group */
    SUDOKU_INVALID_LINE, /* a cell is repeated in a line */
    SUDOKU_INVALID_COLUMN, /* a cell is repeated in a column */
    SUDOKU_UNSOLVABLE, /* it is impossible to solve such board */
};

struct sudoku_cell
{
    uint8_t i;
    uint8_t j;
};

/* Try to solve a sudoku board in-place
 * Returns an error number or 0 on success
 * If the error is SUDOKU_INVALID_*, the coordinates
 * of the faulting cell are stored in badcell by reference.
 * On success, the solution is written in-place. */
enum sudoku_errno solve_sudoku(uint8_t board[9][9], struct sudoku_cell* badcell);

#endif
