/* ===========================================================================
   Exercise 01 — Performance metrics and the roofline, in code

   Every formula from Unit 1, implemented. Once you have written these you will
   use them for the rest of the course: in your labs, your assignments and your
   mini project.

   The formulas are all in the Unit 1 notes. The marks here are for the EDGE
   CASES, which is where every real implementation of these goes wrong:

     * What is the speedup when tN is zero or negative?
     * What is Amdahl's ceiling when the serial fraction is exactly zero?
     * What is Karp-Flatt on one processor?
     * Is a kernel exactly at the ridge point memory bound or compute bound?

   The header states the required answer for each. Read it carefully; the hidden
   tests check every one.

   Rules
     * Do not add a main(). The tests provide one.
     * Do not change include/metrics.h.

   Build and test:  ./selfcheck.sh
   =========================================================================== */

#include "metrics.h"

#include <math.h>

double speedup(double t1, double tN)
{
    /* TODO */
    (void)t1; (void)tN;
    return 0.0;
}

double efficiency(double t1, double tN, int N)
{
    /* TODO */
    (void)t1; (void)tN; (void)N;
    return 0.0;
}

double amdahl(double s, int N)
{
    /* TODO: S = 1 / (s + (1-s)/N) */
    (void)s; (void)N;
    return 0.0;
}

double amdahl_limit(double s)
{
    /* TODO: 1/s, but read the header for what to return when s <= 0 */
    (void)s;
    return 0.0;
}

double gustafson(double s, int N)
{
    /* TODO: S = N - s*(N-1) */
    (void)s; (void)N;
    return 0.0;
}

double karp_flatt(double S, int N)
{
    /* TODO: e = (1/S - 1/N) / (1 - 1/N) */
    (void)S; (void)N;
    return 0.0;
}

double arithmetic_intensity(double flops, double bytes)
{
    /* TODO */
    (void)flops; (void)bytes;
    return 0.0;
}

double ridge_point(double peak_gflops, double bandwidth_gbs)
{
    /* TODO */
    (void)peak_gflops; (void)bandwidth_gbs;
    return 0.0;
}

double attainable_gflops(double peak_gflops, double bandwidth_gbs, double intensity)
{
    /* TODO: min(peak, bandwidth * I) */
    (void)peak_gflops; (void)bandwidth_gbs; (void)intensity;
    return 0.0;
}

int is_memory_bound(double peak_gflops, double bandwidth_gbs, double intensity)
{
    /* TODO: careful at exactly the ridge point, see the header */
    (void)peak_gflops; (void)bandwidth_gbs; (void)intensity;
    return 0;
}

double roofline_fraction(double peak_gflops, double bandwidth_gbs,
                         double intensity, double achieved_gflops)
{
    /* TODO: achieved divided by ATTAINABLE, not by peak */
    (void)peak_gflops; (void)bandwidth_gbs; (void)intensity; (void)achieved_gflops;
    return 0.0;
}
