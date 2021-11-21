#include "sudoku.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    char* programname;
    FILE* testfile;
    int cellvalue;
    int cellabsindex = 0;
    int board[9][9];
    struct sudoku_err err;
    enum sudoku_errno errno;

    if (argc >= 1 && argv[0][0] != '\0')
    {
        programname = argv[0];
    }
    else
    {
        programname = "testsudoku";
    }

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s testfile\n", programname);
        return EXIT_FAILURE;
    }

    testfile = fopen(argv[1], "r");

    if (testfile == NULL)
    {
        perror(programname);
        return EXIT_FAILURE;
    }

    while (cellabsindex < 81 && fscanf(testfile, " %d", &cellvalue) == 1)
    {
        int cell_i = cellabsindex / 9;
        int cell_j = cellabsindex % 9;

        board[cell_i][cell_j] = cellvalue;

        cellabsindex++;
    }

    fclose(testfile);

    if (cellabsindex < 81)
    {
        fprintf(stderr, "%s: Read less than 81 cells\n", programname);
        return EXIT_FAILURE;
    }

    errno = solve_sudoku(board, &err);

    if (errno != SUDOKU_OK)
    {
        switch (errno)
        {
            case SUDOKU_INVALID_CELL:
                fprintf(stderr, "%s: Cell (%d,%d) has invalid value %d\n", programname, err.cell_i + 1, err.cell_j + 1, board[err.cell_i][err.cell_j]);
                break;
            case SUDOKU_INVALID_3X3:
                fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in 3x3 group\n", programname, board[err.cell_i][err.cell_j], err.cell_i + 1, err.cell_j + 1);
                break;
            case SUDOKU_INVALID_LINE:
                fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in line\n", programname, board[err.cell_i][err.cell_j], err.cell_i + 1, err.cell_j + 1);
                break;
            case SUDOKU_INVALID_COLUMN:
                fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in column\n", programname, board[err.cell_i][err.cell_j], err.cell_i + 1, err.cell_j + 1);
                break;
            default:
                fprintf(stderr, "%s: Unknown error number %d returned by solve_sudoku\n", programname, errno);
                break;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
