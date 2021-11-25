#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

static const char* programname;
static int min_num_cells = 18;
static int max_num_cells = 20;
static int board[9][9];

static void print_help()
{
    fprintf(stderr, "Generate random sudoku boards\n"
                    "\n"
                    "Usage: %s [options]\n"
                    "\n"
                    "Options:\n"
                    "  -h        Displays this message and exit immediately\n",
                    programname);
}

static int randint(int min, int max)
{
    return rand() % (max-min+1) + min;
}

int main(int argc, char** argv)
{
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
            char opt;

            while ((opt = *++arg) != '\0')
            {
                switch (opt)
                {
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

    /* Generate board */
    int num_cells = randint(min_num_cells, max_num_cells);

    fprintf(stderr, "n = %d\n", num_cells);

    while (num_cells > 0)
    {
        int i = randint(0, 8);
        int j = randint(0, 8);
        
        if (board[i][j] != 0)
        {
            continue; /* cell already taken */
        }

        int c = randint(1, 9);

        /* group coordinates */
        int gi = i-i%3;
        int gj = j-j%3;
        
        /* is new board valid? */
        bool valid = true;

        for (int k = 0; k < 9; ++k)
        {
            if (board[i][k] == c || board[k][j] == c || board[gi+k/3][gj+k%3] == c)
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            board[i][j] = c;
            --num_cells;
        }
    }

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            printf("%d", board[i][j]);
            
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

    return EXIT_SUCCESS;
}
