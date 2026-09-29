/* Interface you must implement. Do not change this file. */
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stddef.h>

#define NBINS 16

/* Count how many elements of a[0..n-1] fall in each of NBINS equal-width bins
   spanning [lo, hi). Values below lo go in bin 0; values at or above hi go in
   bin NBINS-1. bins[] is zeroed by this function before counting. */
void histogram(const double *a, size_t n, double lo, double hi, long *bins);

#endif
