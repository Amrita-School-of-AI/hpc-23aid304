/* ===========================================================================
   Exercise 17 — First CUDA kernels

   Implement the three functions declared in include/vecops.h. Each one takes
   HOST pointers, so each one must:

       1. allocate device memory            cudaMalloc
       2. copy the inputs across            cudaMemcpy ... HostToDevice
       3. launch a kernel                   kernel<<<blocks, threads>>>(...)
       4. copy the result back              cudaMemcpy ... DeviceToHost
       5. free the device memory            cudaFree

   Rules
     * Do not add a main(). The test harness provides one.
     * Every kernel must use the standard index idiom and a bounds guard:
           int i = blockIdx.x * blockDim.x + threadIdx.x;
           if (i < n) { ... }
       Grid size is computed by ceiling division, so the last block almost
       always contains threads with i >= n. Without the guard they write past
       the end of the allocation and corrupt memory silently.
     * Check your CUDA calls. CUDA_CHECK below is provided; use it.

   Build and test:
       ./selfcheck.sh
   =========================================================================== */

#include "vecops.h"

#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>

#define CUDA_CHECK(call)                                                       \
    do {                                                                       \
        cudaError_t err_ = (call);                                             \
        if (err_ != cudaSuccess) {                                             \
            fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__,                 \
                    cudaGetErrorString(err_));                                 \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

static const int THREADS = 256;

static inline int blocks_for(int n) { return (n + THREADS - 1) / THREADS; }

/* --------------------------------------------------------------------------
   TODO 1: saxpy
   -------------------------------------------------------------------------- */

__global__ void saxpy_kernel(float a, const float *x, const float *y,
                             float *out, int n)
{
    /* your code here */
    (void)a; (void)x; (void)y; (void)out; (void)n;
}

extern "C" void gpu_saxpy(float a, const float *x, const float *y,
                          float *out, int n)
{
    if (n <= 0) return;
    /* your code here: cudaMalloc, cudaMemcpy, launch, cudaMemcpy back, cudaFree */
    (void)a; (void)x; (void)y; (void)out;
}

/* --------------------------------------------------------------------------
   TODO 2: elementwise maximum
   -------------------------------------------------------------------------- */

__global__ void elemmax_kernel(const float *x, const float *y, float *out, int n)
{
    /* your code here */
    (void)x; (void)y; (void)out; (void)n;
}

extern "C" void gpu_elemmax(const float *x, const float *y, float *out, int n)
{
    if (n <= 0) return;
    /* your code here */
    (void)x; (void)y; (void)out;
}

/* --------------------------------------------------------------------------
   TODO 3: count elements strictly greater than t

   A counter shared by every thread is a data race exactly as it was in OpenMP.
   On a GPU the cheap fix is atomicAdd on a single device integer. Any correct
   approach is accepted.
   -------------------------------------------------------------------------- */

__global__ void count_kernel(const float *x, int n, float t, unsigned long long *count)
{
    /* your code here */
    (void)x; (void)n; (void)t; (void)count;
}

extern "C" long gpu_count_above(const float *x, int n, float t)
{
    if (n <= 0) return 0;
    /* your code here */
    (void)x; (void)t;
    return 0;
}
