#!/usr/bin/env bash
# Run the public tests on your own machine before you submit.
#
#     ./selfcheck.sh                 # tests the starter file
#     ./selfcheck.sh AIE23001.cu   # tests a specific file
#
# A pass here means the public tests pass. The grader also runs hidden tests
# so a clean run here is necessary but not sufficient.
set -uo pipefail
cd "$(dirname "$0")"

SRC="${1:-starter/reduce.cu}"
[ -f "$SRC" ] || SRC="reduce.cu"
[ -f "$SRC" ] || { echo "no source file found (looked for starter/reduce.cu and reduce.cu)"; exit 1; }

export PATH=/usr/local/cuda/bin:$PATH
command -v nvcc >/dev/null || { echo "nvcc not found: log in to the cluster first"; exit 1; }

# The harness header sits in a different place in the student repository than in
# the course tree, so look in both rather than hard-coding either.
HARNESS=""
for d in ../harness ../../_toolkit/agrade/harness ../../../_toolkit/agrade/harness; do
  [ -f "$d/atest.h" ] && HARNESS="$d" && break
done
[ -n "$HARNESS" ] || { echo "could not find atest.h"; exit 1; }

echo "==> compiling $SRC"
nvcc -O2 -std=c++17 -arch=sm_80 -Xcompiler "-Wall,-Wextra,-Werror" -I"$HARNESS" -Iinclude "$SRC" tests/public.cu -o .selfcheck -lm || exit 1

echo "==> running public tests"
if command -v srun >/dev/null; then
  echo "   (via srun, since this needs a GPU)"
  srun --gres=gpu:1 --time=00:10:00 --mem=16G ./.selfcheck
else
  ./.selfcheck
fi
rc=$?
rm -f .selfcheck
exit $rc
