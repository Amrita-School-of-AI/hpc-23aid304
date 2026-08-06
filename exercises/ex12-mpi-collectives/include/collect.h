/* Interface you must implement. Do not change this file. */
#ifndef COLLECT_H
#define COLLECT_H

/* All three functions are COLLECTIVE: every rank in MPI_COMM_WORLD calls them.
   Only rank 0 supplies meaningful input in `data`; on other ranks it may be
   NULL. `n` is valid on every rank. */

/* Sum of data[0..n-1]. The result must be returned on EVERY rank. */
double dist_sum(const double *data, int n);

/* Largest element. Returned on EVERY rank. Returns -INFINITY if n == 0. */
double dist_max(const double *data, int n);

/* out[i] = scale * data[i], computed in parallel and collected so that the
   complete result ends up in `out` ON RANK 0. `out` may be NULL elsewhere. */
void dist_scale(const double *data, int n, double scale, double *out);

#endif
