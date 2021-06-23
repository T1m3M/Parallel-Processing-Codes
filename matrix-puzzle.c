#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clear_stream(FILE *in)
{
    int ch;

    clearerr(in);

    do
        ch = getc(in);
    while (ch != '\n' && ch != EOF);

    clearerr(in);
}

int main(int argc, char *argv[])
{
    int rows, cols;
    int i, j, r;

    // accept user input with validation
    printf("# of rows: ");
    fflush(stdout);

    while (scanf("%d", &rows) != 1) {
        clear_stream(stdin);
        printf("Invalid integer!\n # of rows: ");
        fflush(stdout);
    }

    // -------------------------

    printf("# of columns: ");
    fflush(stdout);

    while (scanf("%d", &cols) != 1) {
        clear_stream(stdin);
        printf("Invalid integer!\n # of columns: ");
        fflush(stdout);
    }


    /* make 2D dynamic array */
    char** puzzle = (char**) malloc (rows * sizeof(char*));

    for (i=0; i < rows; i++)
        puzzle[i] = (char*) malloc(cols * sizeof(char));


    srand(time(NULL));

    for (i=0; i < rows; i++)
        for (j=0; j < cols; j++)
        {
            r = rand() % 2;
            if (r == 0)
                puzzle[i][j] = 'O';
            else
                puzzle[i][j] = 'X';
        }


    /* print the puzzle array */
    for (i=0; i < rows; i++)
    {
        for (j=0; j < cols; j++)
            printf("%c ", puzzle[i][j]);
        printf("\n");
    }

    // creating parallel snippet in openMP the puzzle is in shared memory
    // but the counters are private, work will is disturbed statically between threads
    #pragma omp parallel shared(puzzle) private(i, j)
    {
        #pragma omp for schedule(static)
        for (i=0; i < rows; i++)
        {
            for (j=1; j < cols-1; j++)
            {
                // if there's O surrounded by Xs on left and right sides, change it to X
                if (puzzle[i][j-1] == 'X' && puzzle[i][j+1] == 'X' && puzzle[i][j] == 'O')
                    puzzle[i][j] = 'X';

                // for debugging only
                // printf ("IN thread %d puzzle[%d][%d] = %c\n", omp_get_thread_num(), i, j, puzzle[i][j]);
            }

        }
    }     /* end of parallel region */


    /* print the puzzle's solution */
    printf("\n");
    for (i=0; i < rows; i++)
    {
        for (j=0; j < cols; j++)
            printf("%c ", puzzle[i][j]);
        printf("\n");
    }
}
