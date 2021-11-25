#include "sudoku.h"
#include "sudokuaux.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char** argv)
{
    const char* programname;
    bool canclosefile;
    FILE* testfile;
    int status = EXIT_SUCCESS;
    int board[9][9];
    struct sudoku_err err;
    enum sudoku_errno errnum;

    if (argc >= 1 && argv[0][0] != '\0')
    {
        programname = argv[0];
    }
    else
    {
        programname = "sudokusolver";
    }

    if (argc < 2)
    {
        testfile = stdin;
        canclosefile = false;
    }
    else
    {
        testfile = fopen(argv[1], "r");
        canclosefile = true;

        if (testfile == NULL)
        {
            perror(programname);
            return EXIT_FAILURE;
        }
    }

    for (int boardindex = 1; ; ++boardindex)
    {
        int cellvalue;
        int cellabsindex = 0;

        while (cellabsindex < 81 && fscanf(testfile, " %d", &cellvalue) == 1)
        {
            int cell_i = cellabsindex / 9;
            int cell_j = cellabsindex % 9;

            board[cell_i][cell_j] = cellvalue;

            cellabsindex++;
        }

        if (cellabsindex == 0)
        {
            break; /* no more boards */
        }

        if (cellabsindex < 81)
        {
            fprintf(stderr, "%s: Expected board with 81 cells.\n", programname);
            status = EXIT_FAILURE;
            break;
        }

        fprintf(stderr, "Solving board #%d... ", boardindex);

        errnum = solve_sudoku(board, &err);

        fprintf(stderr, "%s\n", statusdescriptions[errnum]);

        if (errnum != SUDOKU_OK)
        {
            break;
        }

        printf("Solution for board #%d\n\n", boardindex);

        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                printf("%d", board[i][j]);
                
                if (j < 8)
                {
                    fputc(' ', stdout);

                    if (j % 3 == 2)
                    {
                        fputc(' ', stdout);
                    }
                }
            }

            fputc('\n', stdout);

            if (i < 8 && i % 3 == 2)
            {
                fputc('\n', stdout);
            }
        }
    }

    if (canclosefile)
    {
        fclose(testfile);
    }

    return status;
}
