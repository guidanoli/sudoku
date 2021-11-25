#include "sudoku.h"

#include <stddef.h>
#include <stdbool.h>

enum sudoku_errno solve_sudoku(int board[9][9], struct sudoku_err* err)
{
    /* check invalid values for each cell */
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            int c = board[i][j];

            if (!(c >= 0 && c <= 9))
            {
                if (err != NULL)
                {
                    err->cell_i = i;
                    err->cell_j = j;
                }

                return SUDOKU_INVALID_CELL;
            }
        }
    }

    /* check for 3x3 cell groups */
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            bool taken[10] = {false};

            for (int k = 0; k < 3; ++k)
            {
                for (int l = 0; l < 3; ++l)
                {
                    int c = board[3*i+k][3*j+l];

                    if (c >= 1 && c <= 9)
                    {
                        if (taken[c])
                        {
                            if (err != NULL)
                            {
                                err->cell_i = 3*i+k;
                                err->cell_j = 3*j+l;
                            }

                            return SUDOKU_INVALID_3X3;
                        }

                        taken[c] = true;
                    }
                }
            }
        }
    }

    /* check for columns of cells */
    for (int i = 0; i < 9; ++i)
    {
        int taken[10] = {false};

        for (int j = 0; j < 9; ++j)
        {
            int c = board[i][j];

            if (c >= 1 && c <= 9)
            {
                if (taken[c])
                {
                    if (err != NULL)
                    {
                        err->cell_i = i;
                        err->cell_j = j;
                    }

                    return SUDOKU_INVALID_COLUMN;
                }

                taken[c] = true;
            }
        }
    }

    /* check for lines of cells */
    for (int j = 0; j < 9; ++j)
    {
        int taken[10] = {false};

        for (int i = 0; i < 9; ++i)
        {
            int c = board[i][j];

            if (c >= 1 && c <= 9)
            {
                if (taken[c])
                {
                    if (err != NULL)
                    {
                        err->cell_i = i;
                        err->cell_j = j;
                    }

                    return SUDOKU_INVALID_LINE;
                }

                taken[c] = true;
            }
        }
    }

    /* Find first non-solved cell */
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (board[i][j] == 0)
            {
                for (int c = 1; c <= 9; ++c)
                {
                    board[i][j] = c;

                    /* Found a solution! */
                    if (solve_sudoku(board, NULL) == SUDOKU_OK)
                    {
                        return SUDOKU_OK;
                    }
                }

                /* Dead-end! */
                board[i][j] = 0;
                return SUDOKU_UNSOLVABLE;
            }
        }
    }

    /* No empty cells! */
    return SUDOKU_OK;
}
