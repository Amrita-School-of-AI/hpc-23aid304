/* ===========================================================================
   Exercise 11 — Domain decomposition with MPI halo exchange

   Solve a 1-D heat diffusion problem by Jacobi iteration, distributed across
   MPI ranks.

       u_new[i] = 0.5 * ( u[i-1] + u[i+1] )

   The global domain has `n` interior cells, numbered 1..n, with fixed boundary
   values u[0] = LEFT_BC and u[n+1] = RIGHT_BC.

   WHAT YOU MUST DO
     * Split the n interior cells across the ranks. n is NOT always divisible
       by the number of ranks: rank r must handle its share correctly anyway.
     * Give each rank a halo cell at each end of its local array.
     * Exchange halos once per timestep using NON-BLOCKING calls
       (MPI_Irecv / MPI_Isend / MPI_Waitall). MPI_Send is rejected by the
       grader, because the symmetric exchange deadlocks with it.
     * Reduce the local interior sums to a global sum on rank 0.

   The verification below is already written. Do not change the output format:
   the grader looks for the exact string "RESULT OK".

   Usage:  mpirun -np <P> ./stencil <n> <steps>
   Build:  mpicc -O2 -std=c11 -Wall -Wextra -Werror -Iinclude stencil.c -o stencil -lm
   =========================================================================== */

#include "stencil.h"

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double reference_sum(int n, int steps)
{
    double *u = calloc((size_t)n + 2, sizeof *u);
    double *v = calloc((size_t)n + 2, sizeof *v);
    if (!u || !v) return NAN;

    u[0] = LEFT_BC;  u[n + 1] = RIGHT_BC;
    v[0] = LEFT_BC;  v[n + 1] = RIGHT_BC;

    for (int s = 0; s < steps; s++) {
        for (int i = 1; i <= n; i++) v[i] = 0.5 * (u[i - 1] + u[i + 1]);
        double *t = u; u = v; v = t;
    }
    double sum = 0.0;
    for (int i = 1; i <= n; i++) sum += u[i];

    free(u); free(v);
    return sum;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n     = (argc > 1) ? atoi(argv[1]) : 240;
    int steps = (argc > 2) ? atoi(argv[2]) : 500;

    /* ---------------------------------------------------------------------
       TODO 1: work out this rank's share of the n interior cells.

       Hint for the uneven case: give the first (n % size) ranks one extra
       cell each. Then local_n differs by at most one between ranks, and the
       shares still add up to exactly n.
       --------------------------------------------------------------------- */
    int local_n = 0;      /* number of interior cells owned by this rank */

    /* ---------------------------------------------------------------------
       TODO 2: allocate local arrays with ONE halo cell at each end, so valid
       interior indices are 1..local_n and the halos are 0 and local_n+1.
       Set the physical boundary values on the first and last rank only.
       --------------------------------------------------------------------- */
    double *u = NULL, *v = NULL;

    /* ---------------------------------------------------------------------
       TODO 3: the time loop.

       Each step:
         a. post MPI_Irecv for both halos, then MPI_Isend of both edges
            (use MPI_PROC_NULL for a neighbour that does not exist: sends to
             it and receives from it succeed immediately and do nothing, which
             removes every boundary special case)
         b. update the interior cells that do not need a halo
         c. MPI_Waitall
         d. update the two edge cells
         e. swap u and v
       --------------------------------------------------------------------- */
    for (int s = 0; s < steps; s++) {
        /* your code here */
        (void)s;
    }

    /* ---------------------------------------------------------------------
       TODO 4: sum this rank's interior cells, then reduce to a global sum
       on rank 0 with MPI_Reduce.
       --------------------------------------------------------------------- */
    double local_sum = 0.0, global_sum = 0.0;

    /* These three lines only keep the starter compiling with -Werror while the
       variables are still unused. Delete them as soon as you use each one. */
    (void)local_n; (void)u; (void)v; (void)local_sum;

    /* ---- verification, already written: do not change the output ---- */
    if (rank == 0) {
        double want = reference_sum(n, steps);
        double denom = fabs(want) > 1.0 ? fabs(want) : 1.0;
        double rel = fabs(want - global_sum) / denom;
        printf("n=%d steps=%d ranks=%d  got=%.10g want=%.10g rel=%.3g\n",
               n, steps, size, global_sum, want, rel);
        if (rel <= 1e-9) printf("RESULT OK\n");
        else             printf("RESULT MISMATCH\n");
    }

    free(u); free(v);
    MPI_Finalize();
    return 0;
}
