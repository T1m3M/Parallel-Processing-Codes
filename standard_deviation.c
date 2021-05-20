#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int my_rank;		/* rank of process */
    int p;			    /* number of process */
    int n;
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

    if( my_rank == 0)
    {

    }
    else
    {
        printf("From P%d n is: %d\n", my_rank, n);
    }

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
