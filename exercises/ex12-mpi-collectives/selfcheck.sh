#!/usr/bin/env bash
# Run the public tests on your own machine before you submit.
#
#     ./selfcheck.sh                 # tests the starter file
#     ./selfcheck.sh AIE23001.c   # tests a specific file
#
# A pass here means the public tests pass. The grader also runs hidden tests
# so a clean run here is necessary but not sufficient.
set -uo pipefail
cd "$(dirname "$0")"

SRC="${1:-starter/collect.c}"
[ -f "$SRC" ] || SRC="collect.c"
[ -f "$SRC" ] || { echo "no source file found (looked for starter/collect.c and collect.c)"; exit 1; }


# The harness header sits in a different place in the student repository than in
# the course tree, so look in both rather than hard-coding either.
HARNESS=""
for d in ../harness ../../_toolkit/agrade/harness ../../../_toolkit/agrade/harness; do
  [ -f "$d/atest.h" ] && HARNESS="$d" && break
done
[ -n "$HARNESS" ] || { echo "could not find atest.h"; exit 1; }

echo "==> compiling $SRC"
mpicc -O2 -std=c11 -Wall -Wextra -Werror -I"$HARNESS" -Iinclude "$SRC" tests/public.c -o .selfcheck -lm || exit 1

echo "==> running public tests"
for p in 1 2 4; do echo "-- $p rank(s)"; mpirun --oversubscribe -np "$p" ./.selfcheck || rc=1; done
rc=$?
rm -f .selfcheck
exit $rc
