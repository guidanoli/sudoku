#include "sudoku.h"

#include <stddef.h>
#include <stdbool.h>

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

    return SUDOKU_OK;
}

static void update_taken(bool taken[10][10][10], int i, int j, int c)
{
    /* 3x3 group */
    {
        int gi = i-i%3, gj = j-j%3;

        for (int gii = 0; gii < 3; ++gii)
        {
            int ii = gi+gii;

            for (int gjj = 0; gjj < 3; ++gjj)
            {
                int jj = gj+gjj;
                
                taken[ii][jj][c] = true;
            }
        }
    }

    /* line */
    for (int ii = 0; ii < 9; ++ii)
    {
        taken[ii][j][c] = true;
    }

    /* column */
    for (int jj = 0; jj < 9; ++jj)
    {
        taken[i][jj][c] = true;
    }
}

enum sudoku_errno solve_sudoku(int board[9][9], struct sudoku_err* err)
{
    /* First, check whether the board is well-formed */
    int status = check_invalid_board(board, err);

    /* We keep a registry of all the taken numbers for every
     * cell in the board. */
    bool taken[10][10][10] = {false};

    /* We also keep which cells are resolved */
    bool resolved[10][10] = {false};

    /* If the board is invalid, we just return */
    if (status != SUDOKU_OK)
    {
        return status;
    }

    /* For every cell in the board, we mark out the cells in the same 3x3 group,
     * in the same line, and in the same column so the value does not repeat */
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            int c = board[i][j];

            if (c != 0)
            {
                resolved[i][j] = true;
                update_taken(taken, i, j, c);
            }
        }
    }

    /* try finding cells with only one taken solution and
     * update the taken and resolved matrices until there is such */
    while (1)
    {
        bool resolved1 = false;

        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                bool manyks = false;
                int c = 0;

                if (resolved[i][j])
                {
                    continue; /* cell is already resolved */
                }

                for (int k = 1; k <= 9; ++k)
                {
                    if (!taken[i][j][k])
                    {
                        if (c == 0)
                        {
                            /* first possible k */
                            c = k;
                        }
                        else
                        {
                            /* second possible k */
                            manyks = true;
                            break;
                        }
                    }
                }

                if (manyks)
                {
                    return SUDOKU_LAZY; /* zzz.. */
                }

                if (c == 0)
                {
                    return SUDOKU_UNSOLVABLE;
                }
                else
                {
                    resolved[i][j] = true;
                    resolved1 = true;
                    update_taken(taken, i, j, c);
                    board[i][j] = c;
                }
            }
        }

        if (!resolved1)
        {
            break;
        }
    }
    
    return SUDOKU_OK;
}
