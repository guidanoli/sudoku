#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

static const char* programname;
static char** argv;
static int argc;
static int min_num_cells = 18;
static int max_num_cells = 30;
static int num_boards = 1;
static uint8_t board[9][9];
static uint16_t taken[9][9];

static void print_board()
{
    for (uint8_t i = 0; i < 9; ++i)
    {
        for (uint8_t j = 0; j < 9; ++j)
        {
            printf("%" PRIu8, board[i][j]);
            
            if (j < 8)
            {
                putchar(' ');

                if (j % 3 == 2)
                {
                    putchar(' ');
                }
            }
        }

        putchar('\n');

        if (i < 8 && i % 3 == 2)
        {
            putchar('\n');
        }
    }
}

static void fill_cell(uint8_t i, uint8_t j, uint8_t c)
{
    uint16_t cmask = (uint16_t)1 << c;

    for (uint8_t k = 0; k < 9; ++k)
    {
        taken[i][k] |= cmask;
        taken[k][j] |= cmask;
    }

    uint8_t inorm = i - i%3;
    uint8_t jnorm = j - j%3;

    for (uint8_t iaux = 0; iaux < 3; ++iaux)
    {
        for (uint8_t jaux = 0; jaux < 3; ++jaux)
        {
            taken[inorm+iaux][jnorm+jaux] |= cmask;
        }
    }

    board[i][j] = c;
}

static int get_option(int opti, int argi)
{
    char* arg, *value, *endptr;
    long int li;
    size_t arglen;

    arg = argv[argi];
    arglen = strlen(arg);

    if (opti == arglen - 1)
    {
        if (argi == argc - 1)
        {
            fprintf(stderr, "%s: Expected value for option '%c'\n", programname, arg[opti]);
            exit(EXIT_FAILURE);
        }
        else
        {
            value = argv[argi+1];
        }
    }
    else
    {
        value = &arg[opti+1];
    }

    li = strtol(value, &endptr, 10);

    if (value[0] == '\0' || endptr[0] != '\0')
    {
        fprintf(stderr, "%s: Expected value for option '%c'\n", programname, arg[opti]);
        exit(EXIT_FAILURE);
    }
    else
    {
        return (int)li;
    }
}

static void print_help()
{
    fprintf(stderr, "Generate random sudoku boards\n"
                    "\n"
                    "Usage: %s [options]\n"
                    "\n"
                    "Options:\n"
                    "  -h             Displays this message and exit immediately\n"
                    "  -c[MIN-CELLS]  Set minimum number of filled cells\n"
                    "  -C[MAX-CELLS]  Set maxmimum number of filled cells\n"
                    "  -n[BOARD-CNT]  Number of boards to be generated\n",
                    programname);
}

static int randint(int min, int max)
{
    return rand() % (max-min+1) + min;
}

int main(int argc_, char** argv_)
{
    /* Get program arguments */
    argv = argv_;
    argc = argc_;

    /* Get program name */
    if (argc >= 1 && argv[0][0] != '\0')
    {
        programname = argv[0];
    }
    else
    {
        programname = "randomsudoku";
    }

    /* Process program arguments */
    for (int argi = 1; argi < argc; ++argi)
    {
        char* arg = argv[argi];

        if (*arg == '-')
        {
            bool processing = true;
            int opti = 1;

            while (processing)
            {
                char opt = arg[opti];

                switch (opt)
                {
                    case '\0':
                        processing = false;
                        break;
                    case 'c':
                        min_num_cells = get_option(opti, argi);
                        processing = false;
                        break;
                    case 'C':
                        max_num_cells = get_option(opti, argi);
                        processing = false;
                        break;
                    case 'n':
                        num_boards = get_option(opti, argi);
                        processing = false;
                        break;
                    case 'h':
                        print_help();
                        return EXIT_SUCCESS;
                    default:
                        fprintf(stderr, "Unknown option '%c'\n", opt);
                        print_help();
                        return EXIT_FAILURE;
                }
            }
        }
    }

    /* Set random seed */
    srand((unsigned int)time(NULL));

    for (int bi = 0; bi < num_boards; ++bi)
    {
        if (bi != 0)
        {
            printf("\n\n");
        }

        /* Clear board and taken */
        memset(board, 0, sizeof(uint8_t)*81);
        memset(taken, 0, sizeof(uint16_t)*81);

        /* Generate board */
        int num_cells = randint(min_num_cells, max_num_cells);

        while (num_cells > 0)
        {
            uint8_t i = (uint8_t)randint(0, 8);
            uint8_t j = (uint8_t)randint(0, 8);
            
            if (board[i][j] != (uint8_t)0)
            {
                continue; /* cell position already taken */
            }

            uint8_t c = (uint8_t)randint(1, 9);

            if (taken[i][j] & ((uint16_t)1 << c))
            {
                continue; /* cell value already taken */
            }

            fill_cell(i, j, c);
            --num_cells;
        }

        print_board();
    }

    return EXIT_SUCCESS;
}
