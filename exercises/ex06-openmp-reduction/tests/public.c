/* Public tests for ex06. These are the same tests the grader runs; the hidden
   set exercises the same behaviour on inputs you have not seen. */
#include "atest.h"
#include "reduction.h"

#include <math.h>
#include <stdlib.h>

static double *fill(size_t n, double (*f)(size_t))
{
    double *a = (double *)malloc(n * sizeof(double));
    for (size_t i = 0; i < n; i++) a[i] = f(i);
    return a;
}

static double ramp(size_t i) { return (double)i; }
static double ones(size_t i) { (void)i; return 1.0; }
static double wave(size_t i) { return sin(0.001 * (double)i); }

TEST(sum_small, "par_sum over 0..999 gives 499500")
{
    double *a = fill(1000, ramp);
    ASSERT_CLOSE(499500.0, par_sum(a, 1000), 1e-12);
    free(a);
}

TEST(sum_empty, "par_sum of an empty array is 0")
{
    ASSERT_NEAR(0.0, par_sum(NULL, 0), 1e-15);
}

TEST(dot_ones, "par_dot of two all-ones arrays of length n is n")
{
    double *a = fill(4096, ones), *b = fill(4096, ones);
    ASSERT_CLOSE(4096.0, par_dot(a, b, 4096), 1e-12);
    free(a); free(b);
}

TEST(max_basic, "par_max finds the largest element")
{
    double a[7] = {-4.0, 2.5, 99.25, 0.0, -1e9, 12.0, 98.0};
    ASSERT_NEAR(99.25, par_max(a, 7), 1e-12);
}

TEST(count_basic, "par_count_above counts strictly greater elements")
{
    double a[6] = {1.0, 2.0, 3.0, 3.0, 4.0, 0.5};
    /* strictly greater than 2.5: 3.0, 3.0, 4.0 */
    ASSERT_EQ_INT(3, par_count_above(a, 6, 2.5));
}

TEST(sum_thread_invariant, "par_sum agrees with a serial sum on 1e6 elements")
{
    size_t n = 1000000;
    double *a = fill(n, wave);
    double want = 0.0;
    for (size_t i = 0; i < n; i++) want += a[i];
    ASSERT_CLOSE(want, par_sum(a, n), 1e-9);
    free(a);
}

int main(void)
{
    REGISTER(sum_small);
    REGISTER(sum_empty);
    REGISTER(dot_ones);
    REGISTER(max_basic);
    REGISTER(count_basic);
    REGISTER(sum_thread_invariant);
    return atest_run_all();
}
