/* Timing driver for the speedup check. */
#include "histogram.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    size_t n = (argc > 1) ? (size_t)strtoull(argv[1], NULL, 10) : 4000000;
    int reps  = (argc > 2) ? atoi(argv[2]) : 40;

    double *a = malloc(n * sizeof *a);
    if (!a) return 1;
    unsigned s = 12345u;
    for (size_t i = 0; i < n; i++) { s = s * 1664525u + 1013904223u; a[i] = (double)(s >> 8) / 16777216.0 * 20.0; }

    long bins[NBINS];
    long total = 0;
    for (int r = 0; r < reps; r++) {
        histogram(a, n, 0.0, 16.0, bins);
        for (int i = 0; i < NBINS; i++) total += bins[i];
    }
    printf("%ld\n", total);
    free(a);
    return 0;
}
