/* Interface you must implement. Do not change this file. */
#ifndef TRANSPOSE_H
#define TRANSPOSE_H

/* b = transpose(a).  a is n x n row-major, b is n x n row-major.
   That is, b[j*n + i] = a[i*n + j]. */
void transpose(const double *a, double *b, int n);

#endif
