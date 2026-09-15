/* Interface you must implement. Do not change this file. */
#ifndef MATMUL_H
#define MATMUL_H

#ifdef __cplusplus
extern "C" {
#endif

/* C = A * B, all row-major host arrays.
   A is m x k, B is k x n, C is m x n.

   naive:  one thread per output element, reading A and B from global memory.
           Provided for you as the baseline to beat.
   tiled:  the same result, but each block stages tiles of A and B in shared
           memory so every loaded element is reused TILE times. This is what
           you implement. */
void gpu_matmul_naive(const float *A, const float *B, float *C,
                      int m, int k, int n);
void gpu_matmul_tiled(const float *A, const float *B, float *C,
                      int m, int k, int n);

#ifdef __cplusplus
}
#endif
#endif
