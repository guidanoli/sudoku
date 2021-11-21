#include "sudoku.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* statusdescriptions[] = {
    [SUDOKU_OK] = "Ok\n",
    [SUDOKU_INVALID_CELL] = "Invalid cell\n",
    [SUDOKU_INVALID_3X3] = "Invalid 3x3\n",
    [SUDOKU_INVALID_LINE] = "Invalid line\n",
    [SUDOKU_INVALID_COLUMN] = "Invalid column\n",
};

int main(int argc, char** argv)
{
    char* programname;
    FILE* testfile;
    int status = EXIT_SUCCESS;
    char buffer[BUFSIZ];
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

    for (int boardindex = 1; ; ++boardindex)
    {
        int cellvalue;
        int cellabsindex = 0;

        fscanf(testfile, " ");

        if (fgets(buffer, sizeof(buffer), testfile) == NULL)
        {
            break; /* No more boards */
        }

        fprintf(stderr, "%s: Reading board #%d...\n", programname, boardindex);

        while (cellabsindex < 81 && fscanf(testfile, " %d", &cellvalue) == 1)
        {
            int cell_i = cellabsindex / 9;
            int cell_j = cellabsindex % 9;

            board[cell_i][cell_j] = cellvalue;

            cellabsindex++;
        }

        if (cellabsindex < 81)
        {
            fprintf(stderr, "%s: Expected board with 81 cells.\n", programname);
            status = EXIT_FAILURE;
            break;
        }

        fprintf(stderr, "%s: Solving board #%d...\n", programname, boardindex);

        errno = solve_sudoku(board, &err);

        if (strcmp(buffer, statusdescriptions[errno]) != 0)
        {
            switch (errno)
            {
                case SUDOKU_OK:
                    fprintf(stderr, "%s: Sudoku solved!\n", programname);
                    break;
                case SUDOKU_INVALID_CELL:
                    fprintf(stderr, "%s: Cell (%d,%d) has invalid value %d.\n", programname, err.cell_i + 1, err.cell_j + 1, board[err.cell_i][err.cell_j]);
                    break;
                case SUDOKU_INVALID_3X3:
                    fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in 3x3 group.\n", programname, board[err.cell_i][err.cell_j], err.cell_i + 1, err.cell_j + 1);
                    break;
                case SUDOKU_INVALID_LINE:
                    fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in line.\n", programname, board[err.cell_i][err.cell_j], err.cell_i + 1, err.cell_j + 1);
                    break;
                case SUDOKU_INVALID_COLUMN:
                    fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in column.\n", programname, board[err.cell_i][err.cell_j], err.cell_i + 1, err.cell_j + 1);
                    break;
                default:
                    fprintf(stderr, "%s: Unknown error number %d returned by solve_sudoku.\n", programname, errno);
                    break;
            }
            status = EXIT_FAILURE;
            break;
        }
    }

    fclose(testfile);

    return status;
}
