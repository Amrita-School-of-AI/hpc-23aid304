# 23AID304 — High-Performance and Cloud Computing

**Dr. Abhijith Anandakrishnan**, Assistant Professor
Amrita School of AI, Amrita Vishwa Vidyapeetham, Coimbatore

L-T-P-C 2-0-2-3 · Semester 5 · B.Tech Artificial Intelligence and Data Science (minor in Medical Engineering)

---

All course material lives here: notes, slides, assignments, lab sheets and coding
exercises. Everything is a PDF built from source in this repository, so it is the
same on every machine and works offline.

## New here? Start with the Getting Started guide

**[handbook/student-guide.pdf](handbook/student-guide.pdf)** takes you from a
laptop with nothing installed to a working setup: a GitHub SSH key, your own fork,
the compiler, and automatic checking of your work on every push. About forty
minutes, once.

Then read **[handbook/course-handbook.pdf](handbook/course-handbook.pdf)** for the
week-by-week plan, the marks breakdown and the policies.


### Handbook

| Document | Size |
|---|---|
| [course handbook](handbook/course-handbook.pdf) | 718 KB |
| [student guide](handbook/student-guide.pdf) | 1054 KB |

### Notes

| Document | Size |
|---|---|
| [unit0 foundations](notes/unit0-foundations.pdf) | 1511 KB |
| [unit1 architecture and performance](notes/unit1-architecture-and-performance.pdf) | 1291 KB |
| [unit2 openmp and mpi](notes/unit2-openmp-and-mpi.pdf) | 1811 KB |
| [unit3 gpu and cuda](notes/unit3-gpu-and-cuda.pdf) | 1444 KB |
| [unit4 cloud and hpc ai](notes/unit4-cloud-and-hpc-ai.pdf) | 1127 KB |

### Slide decks

| Document | Size |
|---|---|
| [d01 why parallel](slides/d01-why-parallel.pdf) | 204 KB |
| [d02 performance and roofline](slides/d02-performance-and-roofline.pdf) | 271 KB |
| [d03 openmp](slides/d03-openmp.pdf) | 270 KB |
| [d04 mpi](slides/d04-mpi.pdf) | 265 KB |
| [d05 gpu and cuda](slides/d05-gpu-and-cuda.pdf) | 302 KB |
| [d06 cloud and hpc ai](slides/d06-cloud-and-hpc-ai.pdf) | 247 KB |

### Assignments

| Document | Size |
|---|---|
| [assignment1 measure before you optimise](assignments/assignment1-measure-before-you-optimise.pdf) | 468 KB |
| [assignment2 openmp scaling](assignments/assignment2-openmp-scaling.pdf) | 325 KB |
| [assignment3 mpi decomposition](assignments/assignment3-mpi-decomposition.pdf) | 315 KB |
| [assignment4 gpu and ai](assignments/assignment4-gpu-and-ai.pdf) | 340 KB |

### Lab sheets

| Document | Size |
|---|---|
| [lab01 timing and memory](labs/lab01-timing-and-memory.pdf) | 595 KB |
| [lab02 slurm and the cluster](labs/lab02-slurm-and-the-cluster.pdf) | 722 KB |
| [lab03 scaling study and roofline](labs/lab03-scaling-study-and-roofline.pdf) | 456 KB |
| [lab04 openmp and races](labs/lab04-openmp-and-races.pdf) | 506 KB |
| [lab05 optimising matrix multiply](labs/lab05-optimising-matrix-multiply.pdf) | 346 KB |
| [lab06 mpi basics and deadlock](labs/lab06-mpi-basics-and-deadlock.pdf) | 398 KB |
| [lab07 domain decomposition](labs/lab07-domain-decomposition.pdf) | 326 KB |
| [lab08 first cuda kernels](labs/lab08-first-cuda-kernels.pdf) | 389 KB |
| [lab09 cuda profiling and roofline](labs/lab09-cuda-profiling-and-roofline.pdf) | 576 KB |
| [HPC_Setup_Guide_Amrita_Vishwa_Vidyapeetham](labs/setup/HPC_Setup_Guide_Amrita_Vishwa_Vidyapeetham.pdf) | 39 KB |

### Coding exercises

| Exercise | Folder |
|---|---|
| [ex01-performance-metrics](exercises/ex01-performance-metrics/ex01-performance-metrics.pdf) | `ex01-performance-metrics` |
| [ex02-cache-blocking](exercises/ex02-cache-blocking/ex02-cache-blocking.pdf) | `ex02-cache-blocking` |
| [ex06-openmp-reduction](exercises/ex06-openmp-reduction/ex06-openmp-reduction.pdf) | `ex06-openmp-reduction` |
| [ex07-openmp-load-balance](exercises/ex07-openmp-load-balance/ex07-openmp-load-balance.pdf) | `ex07-openmp-load-balance` |
| [ex08-openmp-histogram](exercises/ex08-openmp-histogram/ex08-openmp-histogram.pdf) | `ex08-openmp-histogram` |
| [ex11-mpi-halo-stencil](exercises/ex11-mpi-halo-stencil/ex11-mpi-halo-stencil.pdf) | `ex11-mpi-halo-stencil` |
| [ex12-mpi-collectives](exercises/ex12-mpi-collectives/ex12-mpi-collectives.pdf) | `ex12-mpi-collectives` |
| [ex17-cuda-vector-add](exercises/ex17-cuda-vector-add/ex17-cuda-vector-add.pdf) | `ex17-cuda-vector-add` |
| [ex19-cuda-tiled-matmul](exercises/ex19-cuda-tiled-matmul/ex19-cuda-tiled-matmul.pdf) | `ex19-cuda-tiled-matmul` |
| [ex20-cuda-reduction](exercises/ex20-cuda-reduction/ex20-cuda-reduction.pdf) | `ex20-cuda-reduction` |

---

## Working the coding exercises

Each exercise folder contains a problem statement PDF, a `starter/` file to edit,
the **public tests** used to grade part of your submission, and `selfcheck.sh`.

```bash
git clone https://github.com/Amrita-School-of-AI/hpc-23aid304.git
cd hpc-23aid304/exercises/<exercise-name>
./selfcheck.sh
```

When it passes, rename your edited starter file to your **roll number** and submit
that single file:

```
AIE23001.c
```

No archive, no folder. Grading also runs hidden tests, a determinism check across
several thread counts, and where relevant a speedup measurement, so a clean
`selfcheck.sh` is necessary but not sufficient.

## Rebuilding the PDFs

Every document is generated from Markdown by the toolkit in the course repository.
Continuous integration rebuilds them on each push and refuses to publish a PDF that
fails font-embedding or layout checks.

## Licence and reuse

Teaching material for Amrita School of AI. You are welcome to read, learn from and adapt
it with attribution. Please do not redistribute it as your own course.

*Odd Semester 2026-27*
