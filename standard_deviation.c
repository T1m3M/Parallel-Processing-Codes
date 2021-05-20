#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int my_rank;		/* rank of process */
    int p;			    /* number of process */
    int n;
    int i;
    int local_sum;
    int* elements;
    MPI_Status status;	/* return status for receive */

    /* Start up MPI */
    MPI_Init( &argc , &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank == 0)
    {
        printf("number of elements per each process = ");
        scanf("%d", &n);
    }

    /* Broadcasting n to each slave process */
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(my_rank == 0)
    {

    }
    else
    {
        elements = malloc(n * sizeof (int));
        local_sum = 0;

        // different seed for each process
        srand(time(NULL) + my_rank);
        printf("P%d elements are: ", my_rank);

        for(i = 0 ; i < n ; i++){
            elements[i] = rand() % 100;
            printf("%d\t", elements[i]);
            local_sum += elements[i];
        }
        printf("\n");
    }

    //MPI_Allreduce(&local_sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
