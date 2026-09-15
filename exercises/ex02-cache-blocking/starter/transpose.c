/* ===========================================================================
   Exercise 02 — Cache blocking

   Transpose an n x n matrix. No threads, no GPU, no clever mathematics: the
   same 2n^2 memory operations, reordered so the machine can actually keep up.

   The naive version below is correct. It is also slow, and for a reason you can
   state precisely:

       b[j*n + i] = a[i*n + j];

   The read walks a row, so consecutive iterations touch consecutive addresses
   and every 64-byte cache line delivers eight useful doubles. The WRITE walks a
   column, striding n doubles between iterations, so every write touches a fresh
   cache line to use 8 bytes of it. At n = 4096 that stride is 32 KB, so the line
   is long gone by the time the loop returns for its neighbours.

   Your job: reorganise the traversal so that BOTH accesses stay within a small
   working set that fits in cache. Transpose the matrix one square BLOCK at a
   time rather than one element at a time.

       for each block (ii, jj):
           for i in that block:
               for j in that block:
                   b[j*n + i] = a[i*n + j];

   Two b x b blocks must fit in cache together, so 2 * b^2 * 8 bytes should be
   comfortably smaller than your L2. Sizes from 8 to 64 are worth trying; find
   the best one by measuring, not by guessing.

   Rules
     * Do not add a main(). The tests provide one.
     * SINGLE THREADED. No OpenMP, no pthreads. The point of this exercise is
       that memory behaviour is worth a large factor before you add a single
       thread, and parallelising a cache-hostile loop just gives you many cores
       all waiting on memory.
     * n is not always a multiple of your block size. Handle the ragged edge.

   Build and test:  ./selfcheck.sh
   =========================================================================== */

#include "transpose.h"

void transpose(const double *a, double *b, int n)
{
    /* TODO: replace this with a blocked traversal. */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            b[(long)j * n + i] = a[(long)i * n + j];
}
