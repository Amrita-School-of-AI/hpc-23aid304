#include "atest.h"
#include "reduce.h"
#include <math.h>
#include <stdlib.h>

TEST(sum_small, "sum of 0..999")
{
    int n = 1000;
    float *a = (float *)malloc(n * sizeof *a);
    for (int i = 0; i < n; i++) a[i] = (float)i;
    ASSERT_CLOSE(499500.0, gpu_sum(a, n), 1e-5);
    free(a);
}

TEST(sum_not_multiple, "n is prime, so the last block is partly out of range")
{
    int n = 100003;
    float *a = (float *)malloc((size_t)n * sizeof *a);
    for (int i = 0; i < n; i++) a[i] = 1.0f;
    ASSERT_CLOSE((double)n, gpu_sum(a, n), 1e-6);
    free(a);
}

TEST(sum_zero_n, "n = 0 gives 0")
{
    float a[1] = {5.0f};
    ASSERT_NEAR(0.0, gpu_sum(a, 0), 1e-12);
}

TEST(max_basic, "largest element")
{
    float a[7] = {-4.0f, 2.5f, 99.25f, 0.0f, -1e9f, 12.0f, 98.0f};
    ASSERT_NEAR(99.25, gpu_max(a, 7), 1e-4);
}

TEST(max_all_negative, "all-negative input: the identity must not be zero")
{
    int n = 50000;
    float *a = (float *)malloc((size_t)n * sizeof *a);
    for (int i = 0; i < n; i++) a[i] = -1.0f - (float)i;
    ASSERT_NEAR(-1.0, gpu_max(a, n), 1e-4);
    free(a);
}

TEST(max_zero_n, "n = 0 gives -infinity")
{
    float a[1] = {5.0f};
    ASSERT_TRUE(isinf(gpu_max(a, 0)) && gpu_max(a, 0) < 0.0f);
}

int main(void)
{
    REGISTER(sum_small); REGISTER(sum_not_multiple); REGISTER(sum_zero_n);
    REGISTER(max_basic); REGISTER(max_all_negative); REGISTER(max_zero_n);
    return atest_run_all();
}
