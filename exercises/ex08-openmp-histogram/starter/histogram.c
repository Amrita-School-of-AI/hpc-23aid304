/* ===========================================================================
   Exercise 08 — Parallel histogram: contention and false sharing

   Count how many elements fall into each of 16 bins. Serial code below; make it
   scale.

   This exercise has no correctness difficulty at all. Every approach gives the
   right answer. What separates them is speed, and the two obvious approaches are
   both slow for two DIFFERENT reasons.

   Approach 1: share one bins[] array and guard the update.

       #pragma omp parallel for
       for (i = 0; i < n; i++)
       #pragma omp atomic
           bins[bin_of(a[i])]++;

   Correct, and slower than serial. Every element pays for an atomic, and with
   only 16 bins the threads collide constantly. The grader rejects this.

   Approach 2: give each thread its own row of a shared 2-D array.

       long tbins[nthreads][NBINS];        // 16 longs = 128 bytes per thread

   Correct, no atomics, and often still disappointing. 128 bytes is two cache
   lines, so one thread's last bins and the next thread's first bins land on the
   SAME line. Every increment invalidates that line in the neighbouring core.
   That is FALSE SHARING: no data race, no wrong answer, just a line ping-ponging
   between caches.

   What actually works: accumulate into storage that is private to the thread and
   cannot share a line with anyone else's, then combine the per-thread results
   once at the end. There is more than one way to arrange that.

   Rules
     * Do not add a main(). The tests provide one.
     * No #pragma omp critical and no #pragma omp atomic inside the counting
       loop. The grader checks.
     * bins[] must be zeroed before counting, as the serial version does.

   Build and test:
       ./selfcheck.sh
   =========================================================================== */

#include "histogram.h"

#include <omp.h>
#include <stdlib.h>
#include <string.h>

static int bin_of(double v, double lo, double hi)
{
    if (v < lo) return 0;
    if (v >= hi) return NBINS - 1;
    int b = (int)((v - lo) / (hi - lo) * NBINS);
    if (b < 0) b = 0;
    if (b >= NBINS) b = NBINS - 1;
    return b;
}

void histogram(const double *a, size_t n, double lo, double hi, long *bins)
{
    for (int i = 0; i < NBINS; i++) bins[i] = 0;

    /* TODO: parallelise this loop so that it actually scales.
       Think about where each thread's counts live, and what else shares the
       cache line they live on. */
    for (size_t i = 0; i < n; i++) {
        bins[bin_of(a[i], lo, hi)]++;
    }
}
