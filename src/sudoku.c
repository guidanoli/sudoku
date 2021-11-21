#include "sudoku.h"

#include <stddef.h>

static enum sudoku_errno check_invalid_board(const int board[9][9], struct sudoku_err* err)
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
            int present[10] = {0};
            
            for (int k = 0; k < 3; ++k)
            {
                for (int l = 0; l < 3; ++l)
                {
                    int c = board[3*i+k][3*j+l];

                    if (c >= 1 && c <= 9)
                    {
                        if (present[c])
                        {
                            if (err != NULL)
                            {
                                err->cell_i = 3*i+k;
                                err->cell_j = 3*j+l;
                            }

                            return SUDOKU_INVALID_3X3;
                        }

                        present[c] = 1;
                    }
                }
            }
        }
    }

    /* check for columns of cells */
    for (int i = 0; i < 9; ++i)
    {
        int present[10] = {0};

        for (int j = 0; j < 9; ++j)
        {
            int c = board[i][j];

            if (c >= 1 && c <= 9)
            {
                if (present[c])
                {
                    if (err != NULL)
                    {
                        err->cell_i = i;
                        err->cell_j = j;
                    }

                    return SUDOKU_INVALID_COLUMN;
                }

                present[c] = 1;
            }
        }
    }

    /* check for lines of cells */
    for (int j = 0; j < 9; ++j)
    {
        int present[10] = {0};

        for (int i = 0; i < 9; ++i)
        {
            int c = board[i][j];

            if (c >= 1 && c <= 9)
            {
                if (present[c])
                {
                    if (err != NULL)
                    {
                        err->cell_i = i;
                        err->cell_j = j;
                    }

                    return SUDOKU_INVALID_LINE;
                }

                present[c] = 1;
            }
        }
    }

    return SUDOKU_OK;
}

enum sudoku_errno solve_sudoku(int board[9][9], struct sudoku_err* err)
{
    {
        int status = check_invalid_board(board, err);

        if (status != SUDOKU_OK)
        {
            return status;
        }
    }

    return SUDOKU_OK;
}
