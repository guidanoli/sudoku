#include "sudoku.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static enum sudoku_errno check_board(int board[9][9], struct sudoku_err* err)
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
            uint16_t taken = 0;

            for (int k = 0; k < 3; ++k)
            {
                for (int l = 0; l < 3; ++l)
                {
                    int c = board[3*i+k][3*j+l];

                    if (c != 0)
                    {
                        if (taken & (1 << c))
                        {
                            if (err != NULL)
                            {
                                err->cell_i = 3*i+k;
                                err->cell_j = 3*j+l;
                            }

                            return SUDOKU_INVALID_3X3;
                        }

                        taken |= (1 << c);
                    }
                }
            }
        }
    }

    /* check for columns of cells */
    for (int i = 0; i < 9; ++i)
    {
        uint16_t taken = 0;

        for (int j = 0; j < 9; ++j)
        {
            int c = board[i][j];

            if (c != 0)
            {
                if (taken & (1 << c))
                {
                    if (err != NULL)
                    {
                        err->cell_i = i;
                        err->cell_j = j;
                    }

                    return SUDOKU_INVALID_COLUMN;
                }

                taken |= (1 << c);
            }
        }
    }

    /* check for lines of cells */
    for (int j = 0; j < 9; ++j)
    {
        uint16_t taken = 0;

        for (int i = 0; i < 9; ++i)
        {
            int c = board[i][j];

            if (c != 0)
            {
                if (taken & (1 << c))
                {
                    if (err != NULL)
                    {
                        err->cell_i = i;
                        err->cell_j = j;
                    }

                    return SUDOKU_INVALID_LINE;
                }

                taken |= (1 << c);
            }
        }
    }

    return SUDOKU_OK;
}

static enum sudoku_errno solve_sudoku_recursively(int board[9][9])
{
    /* The global matrix of taken numbers
     * each cell [i,j] corresponds to a bitmap of the numbers
     * that are no longer possible to be filled on the board */
    uint16_t gtaken[9][9] = {0};

    /* check for 3x3 cell groups */
    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            uint16_t taken = 0;

            for (int k = 0; k < 3; ++k)
            {
                for (int l = 0; l < 3; ++l)
                {
                    taken |= (1 << board[i+k][j+l]);
                }
            }

            for (int k = 0; k < 3; ++k)
            {
                for (int l = 0; l < 3; ++l)
                {
                    gtaken[i+k][j+l] |= taken;
                }
            }
        }
    }

    /* check for columns of cells */
    for (int i = 0; i < 9; ++i)
    {
        uint16_t taken = 0;

        for (int j = 0; j < 9; ++j)
        {
            taken |= (1 << board[i][j]);
        }

        for (int j = 0; j < 9; ++j)
        {
            gtaken[i][j] |= taken;
        }
    }

    /* check for lines of cells */
    for (int j = 0; j < 9; ++j)
    {
        uint16_t taken = 0;

        for (int i = 0; i < 9; ++i)
        {
            taken |= (1 << board[i][j]);
        }

        for (int i = 0; i < 9; ++i)
        {
            gtaken[i][j] |= taken;
        }
    }

    /* Solve for each cell */
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (board[i][j] == 0)
            {
                uint16_t taken = gtaken[i][j];

                for (int c = 1; c <= 9; ++c)
                {
                    if (!(taken & (1 << c)))
                    {
                        board[i][j] = c;

                        if (solve_sudoku_recursively(board))
                        {
                            /* The board is solved! */
                            return SUDOKU_OK;
                        }
                    }
                }

                /* Could not solve board */
                board[i][j] = 0;
                return SUDOKU_UNSOLVABLE;
            }
        }
    }
    
    /* There are no empty cells -- the board is solved! */
    return SUDOKU_OK;
}

enum sudoku_errno solve_sudoku(int board[9][9], struct sudoku_err* err)
{
    enum sudoku_errno status = check_board(board, err);

    if (status != SUDOKU_OK)
    {
        return status;
    }

    return solve_sudoku_recursively(board);
}
