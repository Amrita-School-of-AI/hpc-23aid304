#!/usr/bin/env bash
# Run the public tests on your own machine before you submit.
#
#     ./selfcheck.sh                 # tests the starter file
#     ./selfcheck.sh AIE23001.c   # tests a specific file
#
# A pass here means the public tests pass. The grader also runs hidden tests, a determinism check across thread counts and a speedup measurement
# so a clean run here is necessary but not sufficient.
set -uo pipefail
cd "$(dirname "$0")"

SRC="${1:-starter/workload.c}"
[ -f "$SRC" ] || SRC="workload.c"
[ -f "$SRC" ] || { echo "no source file found (looked for starter/workload.c and workload.c)"; exit 1; }


# The harness header sits in a different place in the student repository than in
# the course tree, so look in both rather than hard-coding either.
HARNESS=""
for d in ../harness ../../_toolkit/agrade/harness ../../../_toolkit/agrade/harness; do
  [ -f "$d/atest.h" ] && HARNESS="$d" && break
done
[ -n "$HARNESS" ] || { echo "could not find atest.h"; exit 1; }

echo "==> compiling $SRC"
gcc -O2 -std=c11 -Wall -Wextra -Werror -fopenmp -I"$HARNESS" -Iinclude "$SRC" tests/public.c -o .selfcheck -lm || exit 1

echo "==> running public tests"
OMP_NUM_THREADS="${OMP_NUM_THREADS:-4}" ./.selfcheck
rc=$?
rm -f .selfcheck
exit $rc
