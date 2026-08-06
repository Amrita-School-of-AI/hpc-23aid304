/* Shared declarations for the halo-exchange exercise. Do not change this file. */
#ifndef STENCIL_H
#define STENCIL_H

/* Serial reference: returns the sum of the interior after `steps` Jacobi sweeps
   on a domain of `n` interior cells with u[0] = LEFT and u[n+1] = RIGHT.
   Provided so your program can check itself. Do not use it to produce the
   answer: the grader requires real halo exchange. */
double reference_sum(int n, int steps);

#define LEFT_BC  100.0
#define RIGHT_BC   0.0

#endif
