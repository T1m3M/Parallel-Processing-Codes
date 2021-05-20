#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
	int my_rank;		/* rank of process */
	int p;			    /* number of process */
	int source;		    /* rank of sender */
	int dest;		    /* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	MPI_Status status;	/* return status for recieve */

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if(my_rank == 0)
	{
	    int x = 0,
	        y = 100,
	        r = (y - x) / (p - 1);

        for(dest = 1; dest < p ; dest++) {
            int lb, ub;

            // New values for the slave process
            lb = x + (r * (dest - 1));
            ub = x + (r * dest);

            // sending the ranges for each slave process
            MPI_Send(&lb, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send(&ub, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }

        int all_primes = 0;


        for(dest = 1; dest < p ; dest++)
        {
            int each_count;

            // receiving each local count from slave processes
            MPI_Recv(&each_count, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);

            printf("Total number of prime numbers in P%d is: %d\n", dest, each_count);

            all_primes += each_count;
        }

        printf("Total number of prime numbers is: %d\n", all_primes);

	}
	else
	{
	    int lower, upper, local_count = 0, i, is_prime;

	    // receiving the lower and upper bounds
        MPI_Recv(&lower, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&upper, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        // getting all primes in the interval
        while(lower < upper)
        {
            is_prime = 1;

            // 0 and 1 aren't prime numbers (fact)
            if (lower <= 1) {
                lower++;
                continue;
            }

            // factoring to make sure it's a prime
            for (i = 2; i <= lower / 2; i++) {

                if (lower % i == 0) {
                    is_prime = 0;
                    break;
                }
            }

            // if prime is found increase counter
            if (is_prime)
                local_count++;

            // check prime for the next number
            lower++;
        }

        // sending each local count
        MPI_Send(&local_count, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
