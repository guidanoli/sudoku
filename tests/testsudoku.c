#include "sudoku.h"
#include "sudokuaux.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static void remove_endl(char* str)
{
    size_t len = strlen(str);
    size_t i;

    if (len == 0) return;

    /* string is not empty */

    i = len - 1;

    if (str[i] != '\n') return;

    /* string's end contains '\n'
     * and i points to first '\n' known */

    while (i > 0 && str[i-1] == '\n')
    {
        i--;
    }

    str[i] = '\0';
}

int main(int argc, char** argv)
{
    char* programname;
    FILE* testfile;
    int status = EXIT_SUCCESS;
    char buffer[BUFSIZ];
    int board[9][9];
    struct sudoku_err err;
    enum sudoku_errno errnum;

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

        remove_endl(buffer);

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

        printf("Solving board #%d... ", boardindex);

        errnum = solve_sudoku(board, &err);

        printf("%s\n", statusdescriptions[errnum]);

        if (strcmp(buffer, statusdescriptions[errnum]) != 0)
        {
            switch (errnum)
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
                case SUDOKU_UNSOLVABLE:
                    fprintf(stderr, "%s: Board is unsolvable.\n", programname);
                    break;
                case SUDOKU_LAZY:
                    fprintf(stderr, "%s: Too lazy to solve.\n", programname);
                    break;
                default:
                    fprintf(stderr, "%s: Unknown error number %d returned by solve_sudoku.\n", programname, errnum);
                    break;
            }
            status = EXIT_FAILURE;
            break;
        }

        if (errnum == SUDOKU_OK)
        {
            bool matches = true;
            cellabsindex = 0;

            fscanf(testfile, " ");

            while (cellabsindex < 81 && fscanf(testfile, " %d", &cellvalue) == 1)
            {
                int cell_i = cellabsindex / 9;
                int cell_j = cellabsindex % 9;

                if (board[cell_i][cell_j] != cellvalue)
                {
                    fprintf(stderr, "%s: Cell (%d,%d) is different from solution (actual=%d, expected=%d).\n", programname, cell_i + 1, cell_j + 1, board[cell_i][cell_j], cellvalue);
                    matches = false;
                    status = EXIT_FAILURE;
                    break;
                }

                cellabsindex++;
            }

            if (!matches)
            {
                break;
            }

            if (cellabsindex < 81)
            {
                fprintf(stderr, "%s: Expected board with 81 cells.\n", programname);
                status = EXIT_FAILURE;
                break;
            }

        }

    }

    fclose(testfile);

    return status;
}
