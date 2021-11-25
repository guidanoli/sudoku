#include "sudoku.h"
#include "sudokuaux.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

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
    const char* programname;
    FILE* testfile;
    int status = EXIT_SUCCESS;
    char buffer[BUFSIZ];
    uint8_t board[9][9], board2[9][9];
    struct sudoku_cell badcell;
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
        uint8_t cellvalue;
        uint8_t cellabsindex = 0;

        fscanf(testfile, " ");

        if (fgets(buffer, sizeof(buffer), testfile) == NULL)
        {
            break; /* No more boards */
        }

        remove_endl(buffer);

        while (cellabsindex < 81 && fscanf(testfile, " %" SCNu8, &cellvalue) == 1)
        {
            uint8_t cell_i = cellabsindex / 9;
            uint8_t cell_j = cellabsindex % 9;

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

        /* Before solving, store it */
        memcpy(board2, board, sizeof(uint8_t)*81);

        errnum = solve_sudoku(board, &badcell);

        printf("%s\n", statusdescriptions[errnum]);

        if (strcmp(buffer, statusdescriptions[errnum]) != 0)
        {
            switch (errnum)
            {
                case SUDOKU_OK:
                    fprintf(stderr, "%s: Sudoku solved!\n", programname);
                    break;
                case SUDOKU_INVALID_CELL:
                    fprintf(stderr, "%s: Cell (%d,%d) has invalid value %d.\n", programname, badcell.i+1, badcell.j+1, board[badcell.i][badcell.j]);
                    break;
                case SUDOKU_INVALID_3X3:
                    fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in 3x3 group.\n", programname, board[badcell.i][badcell.j], badcell.i+1, badcell.j+1);
                    break;
                case SUDOKU_INVALID_LINE:
                    fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in line.\n", programname, board[badcell.i][badcell.j], badcell.i+1, badcell.j+1);
                    break;
                case SUDOKU_INVALID_COLUMN:
                    fprintf(stderr, "%s: Repeated %d in cell (%d,%d) in column.\n", programname, board[badcell.i][badcell.j], badcell.i+1, badcell.j+1);
                    break;
                case SUDOKU_UNSOLVABLE:
                    fprintf(stderr, "%s: Board is unsolvable.\n", programname);
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
            for (uint8_t i = 0; i < 9; ++i)
            {
                bool taken_line[10] = {false};
                bool taken_column[10] = {false};
                bool taken_group[10] = {false};

                for (uint8_t j = 0; j < 9; ++j)
                {
                    uint8_t c = board[i][j];
                    uint8_t c2 = board2[i][j];
                    uint8_t c3 = board[j][i];
                    uint8_t gi = (i/3)*3 + j/3;
                    uint8_t gj = (i%3)*3 + j%3;
                    uint8_t c4 = board[gi][gj];

                    if (!(c >= 1 && c <= 9))
                    {
                        fprintf(stderr, "%s: Solution has invalid cell (%d,%d)\n", programname, i + 1, j + 1);
                        status = EXIT_FAILURE;
                        break;
                    }

                    if (taken_line[c])
                    {
                        fprintf(stderr, "%s: Solution has invalid line (%d,%d)\n", programname, i + 1, j + 1);
                        status = EXIT_FAILURE;
                        break;
                    }

                    taken_line[c] = true;

                    if (taken_column[c3])
                    {
                        fprintf(stderr, "%s: Solution has invalid line (%d,%d)\n", programname, j + 1, i + 1);
                        status = EXIT_FAILURE;
                        break;
                    }

                    taken_column[c3] = true;

                    if (taken_group[c4])
                    {
                        fprintf(stderr, "%s: Solution has invalid line (%d,%d)\n", programname, gi + 1, gj + 1);
                        status = EXIT_FAILURE;
                        break;
                    }

                    taken_group[c4] = true;

                    if (c2 != 0 && c != c2)
                    {
                        fprintf(stderr, "%s: Solution has overriden cell (%d,%d) -- was %d now is %d\n", programname, i + 1, j + 1, c2, c);
                        status = EXIT_FAILURE;
                        break;
                    }
                }
            }
        }

    }

    fclose(testfile);

    return status;
}
