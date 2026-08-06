/* ===========================================================================
   atest.h — the tiny test harness every C/C++ exercise links against.

   A test file defines cases with TEST(...) and ends with ATEST_MAIN. Each
   assertion that fails prints one line the grader can parse; the process exit
   code is the number of failed cases, capped at 120.

   Deliberately header-only and dependency-free so a student can compile and
   run the public tests on a laptop with nothing but a compiler.

       gcc -O2 -fopenmp student.c tests/public.c -o t && ./t
   =========================================================================== */

#ifndef ATEST_H
#define ATEST_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static int atest_total = 0;
static int atest_failed = 0;
static int atest_case_failed = 0;
static const char *atest_case_name = "";

#define ATEST_MAX_CASES 128
typedef void (*atest_fn)(void);
static atest_fn atest_fns[ATEST_MAX_CASES];
static const char *atest_names[ATEST_MAX_CASES];
static const char *atest_descs[ATEST_MAX_CASES];
static int atest_count = 0;

/* Registration without relying on constructor attributes, so this behaves the
   same under gcc, clang and nvcc. */
#define TEST(name, desc)                                                       \
  static void atest_fn_##name(void);                                           \
  static void atest_reg_##name(void) {                                         \
    if (atest_count < ATEST_MAX_CASES) {                                       \
      atest_fns[atest_count] = atest_fn_##name;                                \
      atest_names[atest_count] = #name;                                        \
      atest_descs[atest_count] = desc;                                         \
      atest_count++;                                                           \
    }                                                                          \
  }                                                                            \
  static void atest_fn_##name(void)

#define REGISTER(name) atest_reg_##name()

/* ---- assertions --------------------------------------------------------- */

#define ATEST_FAIL(fmt, ...)                                                   \
  do {                                                                         \
    atest_case_failed = 1;                                                     \
    printf("  ! %s:%d  " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);          \
  } while (0)

#define ASSERT_TRUE(cond)                                                      \
  do {                                                                         \
    if (!(cond)) ATEST_FAIL("expected true: %s", #cond);                       \
  } while (0)

#define ASSERT_EQ_INT(want, got)                                                \
  do {                                                                         \
    long long _w = (long long)(want), _g = (long long)(got);                    \
    if (_w != _g) ATEST_FAIL("expected %lld, got %lld", _w, _g);                \
  } while (0)

#define ASSERT_NEAR(want, got, tol)                                            \
  do {                                                                         \
    double _w = (double)(want), _g = (double)(got), _t = (double)(tol);         \
    if (!(fabs(_w - _g) <= _t))                                                 \
      ATEST_FAIL("expected %.10g +/- %.3g, got %.10g", _w, _t, _g);             \
  } while (0)

/* Relative comparison, for anything summed in a different order in parallel:
   floating-point addition is not associative, so an exact match is the wrong
   test and would punish a correct answer. */
#define ASSERT_CLOSE(want, got, rel)                                           \
  do {                                                                         \
    double _w = (double)(want), _g = (double)(got), _r = (double)(rel);         \
    double _d = fabs(_w - _g), _s = fabs(_w) > 1.0 ? fabs(_w) : 1.0;            \
    if (!(_d / _s <= _r))                                                       \
      ATEST_FAIL("expected %.10g within %.2g relative, got %.10g", _w, _r, _g);  \
  } while (0)

#define ASSERT_STR_EQ(want, got)                                               \
  do {                                                                         \
    const char *_w = (want), *_g = (got);                                       \
    if (strcmp(_w, _g) != 0) ATEST_FAIL("expected \"%s\", got \"%s\"", _w, _g);  \
  } while (0)

#define ASSERT_ARRAY_CLOSE(want, got, n, rel)                                  \
  do {                                                                         \
    for (int _i = 0; _i < (int)(n); _i++) {                                     \
      double _w = (double)(want)[_i], _g = (double)(got)[_i];                    \
      double _d = fabs(_w - _g), _s = fabs(_w) > 1.0 ? fabs(_w) : 1.0;           \
      if (!(_d / _s <= (double)(rel))) {                                         \
        ATEST_FAIL("element %d: expected %.10g, got %.10g", _i, _w, _g);         \
        break;                                                                   \
      }                                                                          \
    }                                                                            \
  } while (0)

/* ---- runner ------------------------------------------------------------- */

static int atest_run_all(void) {
  for (int i = 0; i < atest_count; i++) {
    atest_case_failed = 0;
    atest_case_name = atest_names[i];
    atest_total++;
    atest_fns[i]();
    if (atest_case_failed) {
      atest_failed++;
      printf("FAIL %s -- %s\n", atest_names[i], atest_descs[i]);
    } else {
      printf("PASS %s -- %s\n", atest_names[i], atest_descs[i]);
    }
    fflush(stdout);
  }
  printf("SUMMARY %d/%d passed\n", atest_total - atest_failed, atest_total);
  return atest_failed > 120 ? 120 : atest_failed;
}

#ifdef __cplusplus
}
#endif

#endif /* ATEST_H */
