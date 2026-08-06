#!/usr/bin/env bash
# Run the public tests on your own machine before you submit.
#
#     ./selfcheck.sh                # tests histogram.c in this folder
#     ./selfcheck.sh AIE23001.c     # tests a specific file
#     OMP_NUM_THREADS=8 ./selfcheck.sh
#
# A pass here means the public tests pass. The grader also runs hidden tests, a
# determinism check across several thread counts, and a speedup measurement, so
# a clean run here is necessary but not sufficient.
set -euo pipefail
cd "$(dirname "$0")"

SRC="${1:-starter/histogram.c}"
[ -f "$SRC" ] || SRC="histogram.c"
[ -f "$SRC" ] || { echo "no source file found (looked for starter/histogram.c and histogram.c)"; exit 1; }

# The harness header sits in a different place in the student repository than in
# the course tree, so look in both rather than hard-coding either.
HARNESS=""
for d in ../harness ../../_toolkit/agrade/harness ../../../_toolkit/agrade/harness; do
  [ -f "$d/atest.h" ] && HARNESS="$d" && break
done
[ -n "$HARNESS" ] || { echo "could not find atest.h"; exit 1; }

echo "==> compiling $SRC"
gcc -O2 -std=c11 -Wall -Wextra -Werror -fopenmp \
    -Iinclude -I"$HARNESS" "$SRC" tests/public.c -o .selfcheck -lm

echo "==> running public tests with OMP_NUM_THREADS=${OMP_NUM_THREADS:-4}"
OMP_NUM_THREADS="${OMP_NUM_THREADS:-4}" ./.selfcheck
rc=$?
rm -f .selfcheck
exit $rc
