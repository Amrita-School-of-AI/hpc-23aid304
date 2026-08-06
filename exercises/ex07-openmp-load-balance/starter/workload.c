/* ===========================================================================
   Exercise 07 — Load imbalance and the schedule clause

   Every row of this workload does a different amount of work: row i performs
   cost(i) inner iterations, and cost grows quadratically with i. Row 2999 costs
   roughly two thousand times what row 0 costs.

   Parallelising the outer loop is one line. Getting it to SCALE is one word.

   With the default static schedule, OpenMP splits the iteration space into
   equal-sized contiguous chunks. Equal in NUMBER of iterations, not in work. On
   four threads, the thread that gets the last quarter of the rows does roughly
   half of all the work in the program, and the other three sit at the barrier
   waiting for it. You bought four cores and you are running at the speed of
   somewhat more than one.

   Your job: make the loop keep every thread busy. The OpenMP schedule clause is
   the tool. Choose between:

       schedule(static)          equal chunks, decided up front, no runtime cost
       schedule(static, k)       round-robin in chunks of k
       schedule(dynamic, k)      threads take the next chunk of k when they finish
       schedule(guided)          large chunks first, shrinking towards the end

   Think about the chunk size too. schedule(dynamic) on its own means a chunk of
   ONE, and when an iteration is cheap the coordination costs more than the work.

   Rules
     * Do not add a main(). The tests provide one.
     * Do not change cost() and do not skip work. Every implementation must
       perform exactly the same arithmetic; only the ASSIGNMENT of rows to
       threads may differ.
     * out[i] must not depend on the thread count.

   Build and test:  ./selfcheck.sh
   =========================================================================== */

#include "workload.h"

#include <math.h>
#include <omp.h>

int cost(int i) { return 1 + (i * i) / 40; }

void run_workload(const double *a, int n, double *out)
{
    /* TODO: parallelise this loop AND balance it. */
    for (int i = 0; i < n; i++) {
        double acc = a[i];
        int k = cost(i);
        for (int j = 0; j < k; j++) acc = acc * 0.999999 + 1e-7 * (double)j;
        out[i] = acc;
    }
}
