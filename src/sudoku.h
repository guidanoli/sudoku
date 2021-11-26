#ifndef sudoku_h
#define sudoku_h

/****************************************************************************
 * Sudoku solver
 * -------------
 * A board of sudoku consists of a 9x9 matrix.
 * Initially, each cell of the matrix can be either empty (represented by 0)
 * or filled (an integer from 1 to 9). Any other value is treated as invalid.
 * Each row, each line and each 3x3 group of the matrix cannot have repeated
 * values, except for 0, which represents empty cells. If so, the board is
 * also considered invalid.
 ***************************************************************************/

#include <stdint.h>
#include <signal.h>

/***************************************************************************
 * Sudoku return value
 * -------------------
 * Returned by solve_sudoku.
 * Indicates whether the function succeeded or not, and, if so, the type
 * of the error that occurred. Further information about the error may
 * be available via the sudoku_cell struct returned by reference.
 **************************************************************************/
enum sudoku_ret
{
    SUDOKU_OK, /* board was solved in-place */
    SUDOKU_INVALID_CELL, /* a cell is not between 0 and 9 */
    SUDOKU_INVALID_3X3, /* a cell is repeated in a 3x3 group */
    SUDOKU_INVALID_LINE, /* a cell is repeated in a line */
    SUDOKU_INVALID_COLUMN, /* a cell is repeated in a column */
    SUDOKU_UNSOLVABLE, /* it is impossible to solve such board */
    SUDOKU_ABORTED, /* abort_solve_sudoku was 1 */
    SUDOKU_MAX, /* number of possible error numbers */
};

/***************************************************************************
 * Sudoku cell
 * -----------
 * Returned by solve_sudoku.
 * Indicates the position of a cell in a sudoku board.
 **************************************************************************/
struct sudoku_cell
{
    uint8_t i; /* row index */
    uint8_t j; /* column index */
};

/* Try to solve a sudoku board in-place
 * Returns an error number or 0 on success
 * If the error is SUDOKU_INVALID_*, the coordinates
 * of the faulting cell are stored in badcell by reference.
 * On success, the solution is written in-place. */

/***************************************************************************
 * Solve sudoku board
 * ------------------
 * Tries to solve a sudoku board.
 * @arg board the initial state of the sudoku board (format described
 *            on the first comment of this header)
 * @arg badcell the address of an existing sudoku_cell struct or NULL
 * @ret SUDOKU_OK if the board was successfully solved, and the solution
 *                is stored in-place on the same board provided
 *      SUDOKU_INVALID_* if the provided board is invalid (if badcell
 *                       is not NULL, stores the location of the faulting
 *                       cell as a (i,j) pair)
 *      SUDOKU_UNSOLVABLE if board cannot be solved
 **************************************************************************/
enum sudoku_ret solve_sudoku(
        uint8_t board[9][9],
        struct sudoku_cell* badcell);

/***************************************************************************
 * solve_signal abort flag
 * -----------------------
 * Can be used by outside code to abort solve_sudoku.
 **************************************************************************/
extern sig_atomic_t abort_solve_sudoku;

#endif
