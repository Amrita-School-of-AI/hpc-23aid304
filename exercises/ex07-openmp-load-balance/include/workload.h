/* Interface you must implement. Do not change this file. */
#ifndef WORKLOAD_H
#define WORKLOAD_H

/* out[i] = cost(i) iterations of a fixed arithmetic kernel seeded from a[i].
   The amount of work for row i grows quadratically with i, so the last rows
   cost far more than the first ones. Deliberately. */
void run_workload(const double *a, int n, double *out);

/* The number of inner iterations row i must perform. Provided, and identical in
   every implementation, so all versions do exactly the same total work. */
int cost(int i);

#endif
