/* Timing driver for the speedup check. */
#include "transpose.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n    = (argc > 1) ? atoi(argv[1]) : 4096;
    int reps = (argc > 2) ? atoi(argv[2]) : 6;

    double *a = malloc((size_t)n * n * sizeof *a);
    double *b = malloc((size_t)n * n * sizeof *b);
    if (!a || !b) return 1;
    for (long i = 0; i < (long)n * n; i++) a[i] = (double)(i % 1000);

    double checksum = 0.0;
    for (int r = 0; r < reps; r++) {
        transpose(a, b, n);
        checksum += b[(long)(n - 1) * n + (n - 1)] + b[1];
    }
    printf("%.1f\n", checksum);
    free(a); free(b);
    return 0;
}
