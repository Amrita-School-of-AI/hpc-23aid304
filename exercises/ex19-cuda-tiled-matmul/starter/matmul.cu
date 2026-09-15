/* ===========================================================================
   Exercise 19 — Shared-memory tiling for matrix multiplication

   gpu_matmul_naive is written for you. It works, and it is slow: every thread
   reads a whole row of A and a whole column of B straight from global memory,
   so each element that arrives is used exactly once. That is an arithmetic
   intensity of about 0.25 FLOP per byte, which Unit 1's roofline model says is
   hopeless.

   Your job is gpu_matmul_tiled. Each block cooperatively loads a TILE x TILE
   tile of A and of B into shared memory, then every thread in the block uses
   those tiles TILE times before they are replaced. Global traffic falls by a
   factor of TILE and the kernel moves from memory bound to compute bound.

   Rules
     * Do not add a main(). The tests provide one.
     * Do not change gpu_matmul_naive: it is the baseline you are measured
       against.
     * Handle m, k and n that are NOT multiples of TILE. Pad with zeros when
       loading a tile that runs off the edge of a matrix.

   The two __syncthreads() calls are both required and they guard different
   things. The first: everyone has finished WRITING the tile before anyone
   READS it. The second: everyone has finished READING it before the next
   iteration OVERWRITES it. Removing either gives intermittently wrong answers,
   more often on larger matrices.

   Build and test:
       ./selfcheck.sh
   =========================================================================== */

#include "matmul.h"

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

#define TILE 32

/* ---------------------------------------------------------------------------
   The baseline. Provided, and not to be changed.
   --------------------------------------------------------------------------- */

__global__ void matmul_naive_kernel(const float *A, const float *B, float *C,
                                    int m, int k, int n)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < m && col < n) {
        float acc = 0.0f;
        for (int t = 0; t < k; t++) acc += A[row * k + t] * B[t * n + col];
        C[row * n + col] = acc;
    }
}

static void launch(const float *A, const float *B, float *C,
                   int m, int k, int n, bool tiled);

extern "C" void gpu_matmul_naive(const float *A, const float *B, float *C,
                                 int m, int k, int n)
{
    launch(A, B, C, m, k, n, false);
}

/* ---------------------------------------------------------------------------
   TODO: the tiled kernel.
   --------------------------------------------------------------------------- */

__global__ void matmul_tiled_kernel(const float *A, const float *B, float *C,
                                    int m, int k, int n)
{
    /* Sketch:

         __shared__ float As[TILE][TILE];
         __shared__ float Bs[TILE][TILE];

         accumulate over ceil(k / TILE) tiles:
             load one element of A and one of B into shared memory,
                 using 0.0f when the index is off the edge
             __syncthreads()
             for t in 0..TILE-1:  acc += As[ty][t] * Bs[t][tx]
             __syncthreads()

         write acc to C if this thread is inside the matrix
    */
    (void)A; (void)B; (void)C; (void)m; (void)k; (void)n;
}

extern "C" void gpu_matmul_tiled(const float *A, const float *B, float *C,
                                 int m, int k, int n)
{
    launch(A, B, C, m, k, n, true);
}

/* ---------------------------------------------------------------------------
   Shared host-side plumbing. Provided.
   --------------------------------------------------------------------------- */

static void launch(const float *A, const float *B, float *C,
                   int m, int k, int n, bool tiled)
{
    if (m <= 0 || k <= 0 || n <= 0) return;

    size_t bytesA = (size_t)m * k * sizeof(float);
    size_t bytesB = (size_t)k * n * sizeof(float);
    size_t bytesC = (size_t)m * n * sizeof(float);

    float *dA, *dB, *dC;
    CUDA_CHECK(cudaMalloc(&dA, bytesA));
    CUDA_CHECK(cudaMalloc(&dB, bytesB));
    CUDA_CHECK(cudaMalloc(&dC, bytesC));
    CUDA_CHECK(cudaMemcpy(dA, A, bytesA, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(dB, B, bytesB, cudaMemcpyHostToDevice));

    dim3 threads(TILE, TILE);
    dim3 blocks((n + TILE - 1) / TILE, (m + TILE - 1) / TILE);

    if (tiled) matmul_tiled_kernel<<<blocks, threads>>>(dA, dB, dC, m, k, n);
    else       matmul_naive_kernel<<<blocks, threads>>>(dA, dB, dC, m, k, n);

    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaMemcpy(C, dC, bytesC, cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(dA)); CUDA_CHECK(cudaFree(dB)); CUDA_CHECK(cudaFree(dC));
}
