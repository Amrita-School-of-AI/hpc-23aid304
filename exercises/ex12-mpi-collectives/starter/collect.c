/* ===========================================================================
   Exercise 12 — MPI collectives

   Three collective operations, each following the same three movements that
   almost every MPI program is built from:

       DISTRIBUTE the data   ->   COMPUTE locally   ->   COMBINE the results

   Only rank 0 holds the input. Every rank must end up with the answer for
   dist_sum and dist_max; only rank 0 needs the full array back from dist_scale.

   THE POINT OF THIS EXERCISE is to use the collective calls rather than writing
   loops of MPI_Send and MPI_Recv. A hand-rolled gather at rank 0 costs P
   communication steps and serialises on one network link; MPI_Allreduce costs
   about log2(P) steps and uses an algorithm tuned to the network. The grader
   rejects a loop of sends.

   n is NOT always divisible by the number of ranks. That is what MPI_Scatterv
   and MPI_Gatherv are for: they take an array of counts and an array of
   displacements instead of one uniform count.

       int *counts = malloc(size * sizeof *counts);
       int *displs = malloc(size * sizeof *displs);
       // counts[r] = n/size + (r < n%size ? 1 : 0);
       // displs[r] = sum of counts[0..r-1]

   Every rank must build these arrays, not just rank 0, because Scatterv and
   Gatherv read them on all ranks in most implementations.

   Rules
     * Do not add a main(). The tests provide one.
     * Every function is collective: every rank calls it, every time.
     * Free anything you allocate.

   Build and test:  ./selfcheck.sh
   =========================================================================== */

#include "collect.h"

#include <math.h>
#include <mpi.h>
#include <stdlib.h>

double dist_sum(const double *data, int n)
{
    /* TODO: scatter, sum locally, Allreduce. */
    (void)data; (void)n;
    return 0.0;
}

double dist_max(const double *data, int n)
{
    /* TODO: same shape, MPI_MAX instead of MPI_SUM.
       Careful: a rank that receives zero elements has no local maximum. */
    (void)data; (void)n;
    return -INFINITY;
}

void dist_scale(const double *data, int n, double scale, double *out)
{
    /* TODO: scatter, scale locally, gather back to rank 0. */
    (void)data; (void)n; (void)scale; (void)out;
}
