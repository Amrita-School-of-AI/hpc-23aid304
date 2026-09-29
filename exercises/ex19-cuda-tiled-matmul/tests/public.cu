/* Public tests for ex19. */
#include "atest.h"
#include "matmul.h"

#include <math.h>
#include <stdlib.h>

static void fill(float *p, int n, unsigned seed)
{
    unsigned s = seed;
    for (int i = 0; i < n; i++) { s = s * 1664525u + 1013904223u; p[i] = (float)((s >> 16) % 17) - 8.0f; }
}

static void cpu_matmul(const float *A, const float *B, float *C, int m, int k, int n)
{
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            float acc = 0.0f;
            for (int t = 0; t < k; t++) acc += A[i * k + t] * B[t * n + j];
            C[i * n + j] = acc;
        }
}

static void check_case(int m, int k, int n, unsigned seed)
{
    float *A = (float *)malloc((size_t)m * k * sizeof(float));
    float *B = (float *)malloc((size_t)k * n * sizeof(float));
    float *C = (float *)malloc((size_t)m * n * sizeof(float));
    float *R = (float *)malloc((size_t)m * n * sizeof(float));
    fill(A, m * k, seed); fill(B, k * n, seed + 77u);
    cpu_matmul(A, B, R, m, k, n);
    for (int i = 0; i < m * n; i++) C[i] = -1e30f;
    gpu_matmul_tiled(A, B, C, m, k, n);
    ASSERT_ARRAY_CLOSE(R, C, m * n, 1e-4);
    free(A); free(B); free(C); free(R);
}

TEST(square_exact_tile, "64 x 64 x 64, an exact multiple of TILE")   { check_case(64, 64, 64, 1u); }
TEST(square_small,      "8 x 8 x 8, smaller than one tile")          { check_case(8, 8, 8, 2u); }
TEST(square_ragged,     "100 x 100 x 100, not a multiple of TILE")   { check_case(100, 100, 100, 3u); }
TEST(rect_wide,         "33 x 65 x 97, all three dimensions ragged") { check_case(33, 65, 97, 4u); }
TEST(single_element,    "1 x 1 x 1")                                 { check_case(1, 1, 1, 5u); }

int main(void)
{
    REGISTER(square_exact_tile);
    REGISTER(square_small);
    REGISTER(square_ragged);
    REGISTER(rect_wide);
    REGISTER(single_element);
    return atest_run_all();
}
