/* Whole-program test for ex12: the collectives must be called by every rank, so
   the harness cannot use the usual per-case model. Rank 0 reports the verdict. */
#include "collect.h"

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;
static int rank, size;

static void expect_near(const char *name, double want, double got, double tol)
{
    if (rank != 0) return;
    if (fabs(want - got) <= tol) { printf("  ok   %s\n", name); }
    else { printf("  FAIL %s: expected %.10g, got %.10g\n", name, want, got); failures++; }
}

/* Every rank must agree on the value, or the "returned on EVERY rank" part of
   the contract is not satisfied. */
static void expect_agreed(const char *name, double v)
{
    double lo, hi;
    MPI_Allreduce(&v, &lo, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&v, &hi, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    if (rank != 0) return;
    if (lo == hi || (isinf(lo) && isinf(hi))) printf("  ok   %s (all ranks agree)\n", name);
    else { printf("  FAIL %s: ranks disagree, %.10g vs %.10g\n", name, lo, hi); failures++; }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* --- n = 1000, chosen so it is not divisible by 3, 4, 5, 6, 7 --- */
    const int n = 1000;
    double *data = NULL, *out = NULL, want_sum = 0.0, want_max = -INFINITY;
    if (rank == 0) {
        data = malloc((size_t)n * sizeof *data);
        out  = malloc((size_t)n * sizeof *out);
        for (int i = 0; i < n; i++) {
            data[i] = (double)((i * 37) % 211) - 100.0;
            want_sum += data[i];
            if (data[i] > want_max) want_max = data[i];
        }
    }
    MPI_Bcast(&want_sum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&want_max, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double s = dist_sum(data, n);
    expect_near("dist_sum on 1000 elements", want_sum, s, 1e-9);
    expect_agreed("dist_sum", s);

    double m = dist_max(data, n);
    expect_near("dist_max on 1000 elements", want_max, m, 0.0);
    expect_agreed("dist_max", m);

    dist_scale(data, n, 2.5, out);
    if (rank == 0) {
        int bad = -1;
        for (int i = 0; i < n; i++)
            if (fabs(out[i] - data[i] * 2.5) > 1e-9) { bad = i; break; }
        if (bad < 0) printf("  ok   dist_scale on 1000 elements\n");
        else { printf("  FAIL dist_scale: element %d wrong\n", bad); failures++; }
    }

    /* --- all-negative input: the identity for MPI_MAX must not be zero --- */
    const int n2 = 97;
    double *neg = NULL, want2 = -INFINITY;
    if (rank == 0) {
        neg = malloc((size_t)n2 * sizeof *neg);
        for (int i = 0; i < n2; i++) { neg[i] = -1.0 - (double)i; if (neg[i] > want2) want2 = neg[i]; }
    }
    MPI_Bcast(&want2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double m2 = dist_max(neg, n2);
    expect_near("dist_max on all-negative input", want2, m2, 0.0);

    /* --- n smaller than the rank count: some ranks get nothing --- */
    const int n3 = 3;
    double *tiny = NULL;
    if (rank == 0) { tiny = malloc(3 * sizeof *tiny); tiny[0] = 5.0; tiny[1] = -2.0; tiny[2] = 9.0; }
    double s3 = dist_sum(tiny, n3);
    expect_near("dist_sum with fewer elements than ranks", 12.0, s3, 1e-12);
    double m3 = dist_max(tiny, n3);
    expect_near("dist_max with fewer elements than ranks", 9.0, m3, 0.0);

    /* --- n = 0 --- */
    double s0 = dist_sum(NULL, 0);
    expect_near("dist_sum with n = 0", 0.0, s0, 0.0);

    if (rank == 0) {
        free(data); free(out); free(neg); free(tiny);
        printf(failures == 0 ? "ALL OK\n" : "FAILURES: %d\n", failures);
    }
    MPI_Finalize();
    return failures ? 1 : 0;
}
