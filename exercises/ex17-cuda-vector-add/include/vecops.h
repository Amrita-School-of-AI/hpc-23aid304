/* Interface you must implement. Do not change this file. */
#ifndef VECOPS_H
#define VECOPS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* out[i] = a * x[i] + y[i]   for i in 0..n-1.
   All three pointers are HOST memory. Your function must move data to the
   device, launch a kernel, and bring the result back. */
void gpu_saxpy(float a, const float *x, const float *y, float *out, int n);

/* out[i] = max(x[i], y[i]) elementwise, host memory in and out. */
void gpu_elemmax(const float *x, const float *y, float *out, int n);

/* Returns the number of elements of x that are strictly greater than t.
   x is host memory. Any correct approach is accepted. */
long gpu_count_above(const float *x, int n, float t);

#ifdef __cplusplus
}
#endif
#endif
