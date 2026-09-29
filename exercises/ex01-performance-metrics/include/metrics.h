/* Interface you must implement. Do not change this file. */
#ifndef METRICS_H
#define METRICS_H

/* --- scaling metrics ---------------------------------------------------- */

/* Speedup S = t1 / tN. Returns 0.0 if tN <= 0. */
double speedup(double t1, double tN);

/* Efficiency E = S / N. Returns 0.0 if N <= 0 or tN <= 0. */
double efficiency(double t1, double tN, int N);

/* Amdahl's law: maximum speedup on N processors with serial fraction s.
   S = 1 / (s + (1-s)/N).  Returns 0.0 if N <= 0 or s outside [0,1]. */
double amdahl(double s, int N);

/* The asymptotic Amdahl ceiling, 1/s. Returns -1.0 for s <= 0 (unbounded). */
double amdahl_limit(double s);

/* Gustafson's law: S = N - s*(N-1). Returns 0.0 if N <= 0 or s outside [0,1]. */
double gustafson(double s, int N);

/* Karp-Flatt experimentally determined serial fraction:
   e = (1/S - 1/N) / (1 - 1/N).  Returns -1.0 if N < 2 or S <= 0. */
double karp_flatt(double S, int N);

/* --- roofline ------------------------------------------------------------ */

/* Arithmetic intensity I = flops / bytes, in FLOP per byte.
   Returns 0.0 if bytes <= 0. */
double arithmetic_intensity(double flops, double bytes);

/* Ridge point of a machine, in FLOP per byte: peak_gflops / bandwidth_gbs.
   Returns 0.0 if bandwidth_gbs <= 0. */
double ridge_point(double peak_gflops, double bandwidth_gbs);

/* Attainable performance in GFLOP/s: min(peak, bandwidth * I). */
double attainable_gflops(double peak_gflops, double bandwidth_gbs, double intensity);

/* 1 if the kernel is memory bound on this machine (I < ridge), else 0.
   A kernel exactly at the ridge counts as compute bound. */
int is_memory_bound(double peak_gflops, double bandwidth_gbs, double intensity);

/* Fraction of the ATTAINABLE ceiling actually achieved, in [0,1].
   Returns 0.0 if the attainable rate is not positive. */
double roofline_fraction(double peak_gflops, double bandwidth_gbs,
                         double intensity, double achieved_gflops);

#endif
