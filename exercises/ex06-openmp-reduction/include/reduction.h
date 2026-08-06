/* Interface you must implement. Do not change this file. */
#ifndef REDUCTION_H
#define REDUCTION_H

#include <stddef.h>

/* Sum of a[0..n-1]. */
double par_sum(const double *a, size_t n);

/* Dot product of a and b, both length n. */
double par_dot(const double *a, const double *b, size_t n);

/* Largest element of a[0..n-1]. Returns -INFINITY when n == 0. */
double par_max(const double *a, size_t n);

/* Number of elements strictly greater than threshold. */
long par_count_above(const double *a, size_t n, double threshold);

#endif
