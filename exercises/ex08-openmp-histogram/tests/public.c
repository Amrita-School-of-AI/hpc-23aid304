#include "atest.h"
#include "histogram.h"
#include <stdlib.h>

static void serial_ref(const double *a, size_t n, double lo, double hi, long *b)
{
    for (int i = 0; i < NBINS; i++) b[i] = 0;
    for (size_t i = 0; i < n; i++) {
        double v = a[i]; int k;
        if (v < lo) k = 0;
        else if (v >= hi) k = NBINS - 1;
        else { k = (int)((v - lo) / (hi - lo) * NBINS); if (k < 0) k = 0; if (k >= NBINS) k = NBINS - 1; }
        b[k]++;
    }
}

TEST(uniform, "uniform data spreads evenly over the bins")
{
    size_t n = 160000;
    double *a = malloc(n * sizeof *a);
    for (size_t i = 0; i < n; i++) a[i] = (double)(i % NBINS) + 0.5;
    long got[NBINS], want[NBINS];
    serial_ref(a, n, 0.0, (double)NBINS, want);
    histogram(a, n, 0.0, (double)NBINS, got);
    for (int i = 0; i < NBINS; i++) { ASSERT_EQ_INT(want[i], got[i]); if (atest_case_failed) break; }
    free(a);
}

TEST(clamps_low_and_high, "values outside [lo,hi) land in the end bins")
{
    double a[6] = {-100.0, -1.0, 0.0, 9.9, 10.0, 1000.0};
    long got[NBINS];
    histogram(a, 6, 0.0, 10.0, got);
    ASSERT_EQ_INT(3, got[0]);              /* -100, -1 clamp in; 0.0 belongs there */
    ASSERT_EQ_INT(3, got[NBINS - 1]);      /* 9.9 rounds to 15; 10.0 and 1000.0 clamp in */
}

TEST(zeroes_first, "bins are zeroed by histogram, not by the caller")
{
    double a[4] = {1.0, 1.0, 1.0, 1.0};
    long got[NBINS];
    for (int i = 0; i < NBINS; i++) got[i] = 999;   /* poison */
    histogram(a, 4, 0.0, 16.0, got);
    long total = 0;
    for (int i = 0; i < NBINS; i++) total += got[i];
    ASSERT_EQ_INT(4, total);
}

TEST(empty, "n = 0 gives all-zero bins")
{
    long got[NBINS];
    histogram(NULL, 0, 0.0, 1.0, got);
    long total = 0;
    for (int i = 0; i < NBINS; i++) total += got[i];
    ASSERT_EQ_INT(0, total);
}

TEST(large_matches_serial, "2e6 elements match the serial reference exactly")
{
    size_t n = 2000000;
    double *a = malloc(n * sizeof *a);
    unsigned s = 7u;
    for (size_t i = 0; i < n; i++) { s = s * 1664525u + 1013904223u; a[i] = (double)(s >> 8) / 16777216.0 * 20.0 - 5.0; }
    long got[NBINS], want[NBINS];
    serial_ref(a, n, 0.0, 10.0, want);
    histogram(a, n, 0.0, 10.0, got);
    for (int i = 0; i < NBINS; i++) { ASSERT_EQ_INT(want[i], got[i]); if (atest_case_failed) break; }
    free(a);
}

int main(void)
{
    REGISTER(uniform); REGISTER(clamps_low_and_high); REGISTER(zeroes_first);
    REGISTER(empty); REGISTER(large_matches_serial);
    return atest_run_all();
}
