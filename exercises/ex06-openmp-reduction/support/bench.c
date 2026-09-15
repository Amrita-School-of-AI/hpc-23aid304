/* Timing driver for the speedup check. Links against whichever implementation
   of reduction.h is being measured.
 *
 * The array is deliberately sized to sit in last-level cache and is swept many
 * times. A single pass over a DRAM-resident array is memory-bandwidth bound, so
 * it would show almost no speedup however well the code is parallelised, and
 * the check would be measuring the memory system rather than the submission.
 */
#include "reduction.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    size_t n    = (argc > 1) ? (size_t)strtoull(argv[1], NULL, 10) : 1000000;
    int    reps = (argc > 2) ? atoi(argv[2]) : 60;

    double *a = (double *)malloc(n * sizeof(double));
    double *b = (double *)malloc(n * sizeof(double));
    if (!a || !b) return 1;

    for (size_t i = 0; i < n; i++) {
        a[i] = (double)((i * 2654435761u) % 1000) / 1000.0;
        b[i] = (double)((i * 40503u) % 997) / 997.0;
    }

    double s = 0.0, d = 0.0, m = 0.0;
    long   c = 0;
    for (int r = 0; r < reps; r++) {
        s += par_sum(a, n);
        d += par_dot(a, b, n);
        m += par_max(a, n);
        c += par_count_above(a, n, 0.5);
    }

    printf("%.6f %.6f %.6f %ld\n", s, d, m, c);
    free(a);
    free(b);
    return 0;
}
