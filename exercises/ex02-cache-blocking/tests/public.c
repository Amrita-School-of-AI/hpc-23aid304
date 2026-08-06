#include "atest.h"
#include "transpose.h"
#include <stdlib.h>

static void check_n(int n)
{
    double *a = malloc((size_t)n * n * sizeof *a);
    double *b = malloc((size_t)n * n * sizeof *b);
    for (long i = 0; i < (long)n * n; i++) { a[i] = (double)i; b[i] = -1.0; }
    transpose(a, b, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            ASSERT_NEAR(a[(long)i * n + j], b[(long)j * n + i], 0.0);
            if (atest_case_failed) { free(a); free(b); return; }
        }
    free(a); free(b);
}

TEST(t_1,   "n = 1")            { check_n(1); }
TEST(t_2,   "n = 2")            { check_n(2); }
TEST(t_32,  "n = 32, an exact multiple of a typical block") { check_n(32); }
TEST(t_33,  "n = 33, one past a block boundary")            { check_n(33); }
TEST(t_100, "n = 100")          { check_n(100); }

TEST(involution, "transposing twice gives back the original")
{
    int n = 65;
    double *a = malloc((size_t)n * n * sizeof *a);
    double *b = malloc((size_t)n * n * sizeof *b);
    double *c = malloc((size_t)n * n * sizeof *c);
    for (long i = 0; i < (long)n * n; i++) a[i] = (double)(i * 7 % 101);
    transpose(a, b, n);
    transpose(b, c, n);
    ASSERT_ARRAY_CLOSE(a, c, n * n, 0.0);
    free(a); free(b); free(c);
}

int main(void)
{
    REGISTER(t_1); REGISTER(t_2); REGISTER(t_32); REGISTER(t_33); REGISTER(t_100);
    REGISTER(involution);
    return atest_run_all();
}
