#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int my_rank;		/* rank of process */
    int p;			    /* number of process */
    int n;
    int i;
    int local_sum = 0, global_sum = 0;
    int* elements;
    float global_mean = 0, local_sum_sqrd_diff = 0, global_sum_sqrd_diff = 0;
    double variance, standard_deviation;
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

    if(my_rank != 0)
    {
        elements = malloc(n * sizeof (int));
        local_sum = 0;

        // Different seed for each process
        srand(time(NULL) + my_rank);
        printf("P%d elements are: ", my_rank);

        // Generating random elements for each process
        for(i = 0 ; i < n ; i++){
            elements[i] = rand() % 100;
            printf("%d\t", elements[i]);
            local_sum += elements[i];
        }
        printf("\n");
    }

    /* Reducing sum of all local sums to global sum */
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


    if(my_rank != 0)
    {
        local_sum_sqrd_diff = 0;

        // Calculating the global mean
        global_mean = ((float)global_sum) / (n * (p - 1));

        // Calculating the local sum of squared differences
        for(i = 0 ; i < n ; i++)
            local_sum_sqrd_diff += pow(elements[i] - global_mean, 2);
    }

    /* Reducing local sum of squared difference to global sum */
    MPI_Reduce(&local_sum_sqrd_diff, &global_sum_sqrd_diff, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(my_rank == 0)
    {
        variance = global_sum_sqrd_diff / (n * (p - 1));
        printf("\nVariance = %.2lf\n", variance);

        standard_deviation = sqrt(variance);
        printf("Standard Deviation = %.2lf\n", standard_deviation);
    }

    /* shutdown MPI */
    MPI_Finalize();

    return 0;
}
