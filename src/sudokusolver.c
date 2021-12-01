#include "sudoku.h"
#include "sudokuaux.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <signal.h>

static uint8_t board[9][9];
static FILE* outfile;

static void sigint_handler(int signo)
{
    abort_solve_sudoku = 1;
}

static void print_board()
{
    fputc('\n', outfile);

    for (uint8_t i = 0; i < 9; ++i)
    {
        for (uint8_t j = 0; j < 9; ++j)
        {
            fprintf(outfile, "%d", board[i][j]);
            
            if (j < 8)
            {
                fputc(' ', outfile);

                if (j % 3 == 2)
                {
                    fputc(' ', outfile);
                }
            }
        }

        fputc('\n', outfile);

        if (i < 8 && i % 3 == 2)
        {
            fputc('\n', outfile);
        }
    }
}

int main(int argc, char** argv)
{
    const char* programname;
    bool canclosetestfile, cancloseoutfile;
    FILE* testfile;
    int status = EXIT_SUCCESS;
    struct sudoku_cell badcell;
    enum sudoku_ret errnum;

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
        canclosetestfile = false;
    }
    else
    {
        testfile = fopen(argv[1], "r");

        if (testfile == NULL)
        {
            perror(programname);
            return EXIT_FAILURE;
        }

        canclosetestfile = true;
    }

    if (argc < 3)
    {
        outfile = stdout;
        cancloseoutfile = false;
    }
    else
    {
        outfile = fopen(argv[2], "w");

        if (outfile == NULL)
        {
            perror(programname);
            return EXIT_FAILURE;
        }

        cancloseoutfile = true;
    }

    signal(SIGINT, sigint_handler);

    for (int boardindex = 1; ; ++boardindex)
    {
        uint8_t value;
        uint8_t absindex = 0;
        uint8_t i, j;

        while (absindex < 81 && fscanf(testfile, " %" SCNu8, &value) == 1)
        {
            i = absindex / 9;
            j = absindex % 9;

            board[i][j] = value;

            absindex++;
        }

        if (absindex == 0)
        {
            break; /* no more boards */
        }

        if (absindex < 81)
        {
            fprintf(outfile, "%s: Expected board with 81 cells.\n", programname);
            status = EXIT_FAILURE;
            break;
        }

        if (boardindex > 1)
        {
            fputc('\n', outfile);
        }

        fprintf(outfile, "Board #%d: ", boardindex);

        errnum = solve_sudoku(board, &badcell);

        fprintf(outfile, "%s\n", statusdescriptions[errnum]);

        switch (errnum)
        {
            case SUDOKU_OK:
                print_board();
                break;
            case SUDOKU_INVALID_CELL:
            case SUDOKU_INVALID_3X3:
            case SUDOKU_INVALID_LINE:
            case SUDOKU_INVALID_COLUMN:
                fprintf(outfile, "Bad cell at (%" PRIu8 ",%" PRIu8 ") with value %" PRIu8 "\n",
                        badcell.i + 1, badcell.j + 1, board[badcell.i][badcell.j]);
                break;
        }

        if (errnum == SUDOKU_ABORTED)
        {
            status = EXIT_FAILURE;
            break;
        }
    }

    if (canclosetestfile)
    {
        fclose(testfile);
        testfile = NULL;
    }

    if (cancloseoutfile)
    {
        fclose(outfile);
        outfile = NULL;
    }

    return status;
}
