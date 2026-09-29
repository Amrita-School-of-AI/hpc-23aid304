/* ===========================================================================
   Exercise 06 — Parallel reduction with OpenMP

   Implement the four functions declared in include/reduction.h so that each
   one uses OpenMP to compute its result in parallel.

   Rules
     * Do not add a main() function. The test harness provides one.
     * Do not change include/reduction.h.
     * Every function must give the same answer regardless of thread count.

   Build and test locally:
       ./selfcheck.sh
   =========================================================================== */

#include "reduction.h"

#include <math.h>
#include <omp.h>

double par_sum(const double *a, size_t n)
{
    double sum = 0.0;

    /* TODO: parallelise this loop.
       Think about what happens to `sum` when eight threads write to it at the
       same time, and which OpenMP clause fixes that without a critical section. */
    for (size_t i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum;
}

double par_dot(const double *a, const double *b, size_t n)
{
    double dot = 0.0;

    /* TODO: same idea, two arrays. */
    for (size_t i = 0; i < n; i++) {
        dot += a[i] * b[i];
    }

    return dot;
}

double par_max(const double *a, size_t n)
{
    if (n == 0) {
        return -INFINITY;
    }

    double best = a[0];

    /* TODO: OpenMP can reduce with operators other than +. Which one applies? */
    for (size_t i = 1; i < n; i++) {
        if (a[i] > best) {
            best = a[i];
        }
    }

    return best;
}

long par_count_above(const double *a, size_t n, double threshold)
{
    long count = 0;

    /* TODO: a counter is a reduction too. */
    for (size_t i = 0; i < n; i++) {
        if (a[i] > threshold) {
            count++;
        }
    }

    return count;
}
