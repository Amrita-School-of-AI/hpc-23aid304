/* Interface you must implement. Do not change this file. */
#ifndef REDUCE_H
#define REDUCE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Sum of in[0..n-1]. `in` is HOST memory; move it to the device, reduce it
   there, and return the total. Returns 0.0 for n <= 0. */
double gpu_sum(const float *in, int n);

/* Largest element of in[0..n-1]. Host memory in.
   Returns -INFINITY for n <= 0. */
float gpu_max(const float *in, int n);

#ifdef __cplusplus
}
#endif
#endif
