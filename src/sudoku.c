#include "sudoku.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

sig_atomic_t abort_solve_sudoku;

static enum sudoku_ret check_board(uint8_t board[9][9], struct sudoku_cell* badcell)
{
    /* check invalid values for each cell */
    for (uint8_t i = 0; i < 9; ++i)
    {
        for (uint8_t j = 0; j < 9; ++j)
        {
            uint8_t c = board[i][j];

            if (!(c >= 0 && c <= 9))
            {
                if (badcell != NULL)
                {
                    badcell->i = i;
                    badcell->j = j;
                }

                return SUDOKU_INVALID_CELL;
            }
        }
    }

    /* check for 3x3 cell groups */
    for (uint8_t i = 0; i < 9; i += 3)
    {
        for (uint8_t j = 0; j < 9; j += 3)
        {
            uint16_t taken = 0;

            for (uint8_t k = 0; k < 3; ++k)
            {
                for (uint8_t l = 0; l < 3; ++l)
                {
                    uint8_t c = board[i+k][j+l];

                    if (c != 0)
                    {
                        if (taken & ((uint16_t)1 << c))
                        {
                            if (badcell != NULL)
                            {
                                badcell->i = i+k;
                                badcell->j = j+l;
                            }

                            return SUDOKU_INVALID_3X3;
                        }

                        taken |= ((uint16_t)1 << c);
                    }
                }
            }
        }
    }

    /* check for columns of cells */
    for (uint8_t i = 0; i < 9; ++i)
    {
        uint16_t taken = 0;

        for (uint8_t j = 0; j < 9; ++j)
        {
            uint8_t c = board[i][j];

            if (c != 0)
            {
                if (taken & ((uint16_t)1 << c))
                {
                    if (badcell != NULL)
                    {
                        badcell->i = i;
                        badcell->j = j;
                    }

                    return SUDOKU_INVALID_COLUMN;
                }

                taken |= ((uint16_t)1 << c);
            }
        }
    }

    /* check for lines of cells */
    for (uint8_t j = 0; j < 9; ++j)
    {
        uint16_t taken = 0;

        for (uint8_t i = 0; i < 9; ++i)
        {
            uint8_t c = board[i][j];

            if (c != 0)
            {
                if (taken & ((uint16_t)1 << c))
                {
                    if (badcell != NULL)
                    {
                        badcell->i = i;
                        badcell->j = j;
                    }

                    return SUDOKU_INVALID_LINE;
                }

                taken |= ((uint16_t)1 << c);
            }
        }
    }

    return SUDOKU_OK;
}

static enum sudoku_ret solve_sudoku_recursively(uint8_t board[9][9])
{
    /* Return value */
    enum sudoku_ret ret;

    /* The global matrix of taken numbers
     * each cell [i,j] corresponds to a bitmap of the numbers
     * that are no longer possible to be filled on the board */
    uint16_t gtaken[9][9] = {0};

    /* check if abort flag is turned on */
    if (abort_solve_sudoku)
    {
        return SUDOKU_ABORTED;
    }

    /* check for 3x3 cell groups */
    for (uint8_t i = 0; i < 9; i += 3)
    {
        for (uint8_t j = 0; j < 9; j += 3)
        {
            uint16_t taken = 0;

            for (uint8_t k = 0; k < 3; ++k)
            {
                for (uint8_t l = 0; l < 3; ++l)
                {
                    taken |= ((uint16_t)1 << board[i+k][j+l]);
                }
            }

            for (uint8_t k = 0; k < 3; ++k)
            {
                for (uint8_t l = 0; l < 3; ++l)
                {
                    gtaken[i+k][j+l] |= taken;
                }
            }
        }
    }

    /* check for columns of cells */
    for (uint8_t i = 0; i < 9; ++i)
    {
        uint16_t taken = 0;

        for (uint8_t j = 0; j < 9; ++j)
        {
            taken |= ((uint16_t)1 << board[i][j]);
        }

        for (uint8_t j = 0; j < 9; ++j)
        {
            gtaken[i][j] |= taken;
        }
    }

    /* check for lines of cells */
    for (uint8_t j = 0; j < 9; ++j)
    {
        uint16_t taken = 0;

        for (uint8_t i = 0; i < 9; ++i)
        {
            taken |= ((uint16_t)1 << board[i][j]);
        }

        for (uint8_t i = 0; i < 9; ++i)
        {
            gtaken[i][j] |= taken;
        }
    }

    /* Solve for each cell */
    for (uint8_t i = 0; i < 9; ++i)
    {
        for (uint8_t j = 0; j < 9; ++j)
        {
            if (board[i][j] == 0)
            {
                uint16_t taken = gtaken[i][j];

                for (uint8_t c = 1; c <= 9; ++c)
                {
                    if (!(taken & ((uint16_t)1 << c)))
                    {
                        board[i][j] = c;

                        ret = solve_sudoku_recursively(board);

                        if (ret == SUDOKU_OK)
                        {
                            /* The board is solved! */
                            return SUDOKU_OK;
                        }
                        else if (ret == SUDOKU_ABORTED)
                        {
                            /* Propagate abortion */
                            board[i][j] = 0;
                            return SUDOKU_ABORTED;
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

enum sudoku_ret solve_sudoku(uint8_t board[9][9], struct sudoku_cell* badcell)
{
    enum sudoku_ret status = check_board(board, badcell);

    if (status != SUDOKU_OK)
    {
        return status;
    }

    return solve_sudoku_recursively(board);
}
