/* Public tests for ex17. */
#include "atest.h"
#include "vecops.h"

#include <math.h>
#include <stdlib.h>

TEST(saxpy_small, "gpu_saxpy on 1000 elements")
{
    int n = 1000;
    float *x = (float *)malloc(n * sizeof *x);
    float *y = (float *)malloc(n * sizeof *y);
    float *o = (float *)malloc(n * sizeof *o);
    for (int i = 0; i < n; i++) { x[i] = (float)i; y[i] = 2.0f * (float)i; }
    gpu_saxpy(3.0f, x, y, o, n);
    for (int i = 0; i < n; i++) {
        ASSERT_NEAR(3.0 * i + 2.0 * i, o[i], 1e-3);
        if (atest_case_failed) break;
    }
    free(x); free(y); free(o);
}

TEST(saxpy_not_multiple, "gpu_saxpy when n is not a multiple of the block size")
{
    int n = 1000003;              /* prime: the last block is partly out of range */
    float *x = (float *)malloc((size_t)n * sizeof *x);
    float *y = (float *)malloc((size_t)n * sizeof *y);
    float *o = (float *)malloc((size_t)n * sizeof *o);
    for (int i = 0; i < n; i++) { x[i] = 1.0f; y[i] = 0.5f; }
    for (int i = 0; i < n; i++) o[i] = -1.0f;
    gpu_saxpy(2.0f, x, y, o, n);
    ASSERT_NEAR(2.5, o[0], 1e-5);
    ASSERT_NEAR(2.5, o[n - 1], 1e-5);
    ASSERT_NEAR(2.5, o[n / 2], 1e-5);
    free(x); free(y); free(o);
}

TEST(elemmax_basic, "gpu_elemmax picks the larger of each pair")
{
    int n = 5;
    float x[5] = {1.0f, -2.0f, 3.5f, 0.0f, 9.0f};
    float y[5] = {2.0f, -5.0f, 3.0f, 0.5f, 8.0f};
    float o[5];
    gpu_elemmax(x, y, o, n);
    ASSERT_NEAR(2.0, o[0], 1e-6);
    ASSERT_NEAR(-2.0, o[1], 1e-6);
    ASSERT_NEAR(3.5, o[2], 1e-6);
    ASSERT_NEAR(0.5, o[3], 1e-6);
    ASSERT_NEAR(9.0, o[4], 1e-6);
}

TEST(count_basic, "gpu_count_above is strict, not inclusive")
{
    float x[6] = {1.0f, 2.0f, 3.0f, 3.0f, 4.0f, 0.5f};
    ASSERT_EQ_INT(3, gpu_count_above(x, 6, 2.5f));
}

TEST(count_zero, "gpu_count_above returns 0 when nothing qualifies")
{
    float x[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    ASSERT_EQ_INT(0, gpu_count_above(x, 4, 5.0f));
}

int main(void)
{
    REGISTER(saxpy_small);
    REGISTER(saxpy_not_multiple);
    REGISTER(elemmax_basic);
    REGISTER(count_basic);
    REGISTER(count_zero);
    return atest_run_all();
}
