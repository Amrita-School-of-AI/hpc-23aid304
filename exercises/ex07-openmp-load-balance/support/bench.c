#include "workload.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n = (argc > 1) ? atoi(argv[1]) : 3000;
    double *a = malloc((size_t)n * sizeof *a);
    double *o = malloc((size_t)n * sizeof *o);
    if (!a || !o) return 1;
    for (int i = 0; i < n; i++) a[i] = 1.0 + (double)(i % 17);
    run_workload(a, n, o);
    double s = 0.0;
    for (int i = 0; i < n; i++) s += o[i];
    printf("%.6f\n", s);
    free(a); free(o);
    return 0;
}
