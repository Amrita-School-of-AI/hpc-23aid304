#include "atest.h"
#include "workload.h"
#include <math.h>
#include <stdlib.h>

static void serial_ref(const double *a, int n, double *out)
{
    for (int i = 0; i < n; i++) {
        double acc = a[i];
        int k = cost(i);
        for (int j = 0; j < k; j++) acc = acc * 0.999999 + 1e-7 * (double)j;
        out[i] = acc;
    }
}

static void compare(int n)
{
    double *a = malloc((size_t)n * sizeof *a);
    double *g = malloc((size_t)n * sizeof *g);
    double *w = malloc((size_t)n * sizeof *w);
    for (int i = 0; i < n; i++) { a[i] = 1.0 + (double)(i % 17); g[i] = -1.0; }
    serial_ref(a, n, w);
    run_workload(a, n, g);
    for (int i = 0; i < n; i++) { ASSERT_NEAR(w[i], g[i], 1e-12); if (atest_case_failed) break; }
    free(a); free(g); free(w);
}

TEST(w_small,  "n = 1")       { compare(1); }
TEST(w_10,     "n = 10")      { compare(10); }
TEST(w_257,    "n = 257, prime")  { compare(257); }
TEST(w_1000,   "n = 1000")    { compare(1000); }
TEST(cost_untouched, "cost() still returns the expected values")
{
    ASSERT_EQ_INT(1, cost(0));
    ASSERT_EQ_INT(3, cost(10));
    ASSERT_EQ_INT(1 + (2999 * 2999) / 40, cost(2999));
}

int main(void)
{
    REGISTER(w_small); REGISTER(w_10); REGISTER(w_257); REGISTER(w_1000);
    REGISTER(cost_untouched);
    return atest_run_all();
}
