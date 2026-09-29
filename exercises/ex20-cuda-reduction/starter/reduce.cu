/* ===========================================================================
   Exercise 20 — Parallel reduction on a GPU

   Sum an array, and find its maximum, on the device.

   This is easy on a CPU and instructive on a GPU, because of one hard fact:

       THERE IS NO SYNCHRONISATION BETWEEN THREAD BLOCKS.

   A block cannot wait for other blocks, so no single kernel launch can combine
   contributions from the whole grid. The standard answer is a TWO-STAGE
   reduction:

       stage 1: each block reduces its own slice and writes ONE partial result
       stage 2: a second launch (or a small final pass) reduces the partials

   The kernel launch boundary is the global barrier you cannot get any other way.

   Within a block, reduce in shared memory as a tree:

       __shared__ float sdata[BLOCK];
       sdata[tid] = my_value;
       __syncthreads();
       for (unsigned s = blockDim.x / 2; s > 0; s >>= 1) {
           if (tid < s) sdata[tid] += sdata[tid + s];
           __syncthreads();                 // EVERY thread reaches this
       }
       if (tid == 0) out[blockIdx.x] = sdata[0];

   Note the shape of the loop. Halving the stride and keeping the ACTIVE threads
   contiguous (tid < s) means whole warps go idle together instead of every warp
   being half idle, which is the difference between a divergent reduction and a
   clean one.

   Rules
     * Do not add a main(). The tests provide one.
     * Use a shared-memory tree reduction. A per-element atomicAdd on the input
       is rejected: it works, and it is not what this exercise teaches.
     * n is not a multiple of the block size. Threads past the end must
       contribute the IDENTITY of the operator: 0 for a sum, -INFINITY for a
       maximum. Contributing 0 to a maximum breaks all-negative inputs.
     * __syncthreads() must be reached by every thread in the block. Never put
       it inside a divergent branch.

   Build and test:  ./selfcheck.sh
   =========================================================================== */

#include "reduce.h"

#include <cuda_runtime.h>
#include <math.h>
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

#define BLOCK 256

/* TODO: your kernels here. */

extern "C" double gpu_sum(const float *in, int n)
{
    if (n <= 0) return 0.0;
    /* TODO */
    (void)in;
    return 0.0;
}

extern "C" float gpu_max(const float *in, int n)
{
    if (n <= 0) return -INFINITY;
    /* TODO */
    (void)in;
    return -INFINITY;
}
