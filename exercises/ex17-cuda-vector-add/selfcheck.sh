#!/usr/bin/env bash
# Run the public tests. Needs a GPU, so on asaicompute this goes through SLURM.
#
#     ./selfcheck.sh                # tests the starter file
#     ./selfcheck.sh AIE23001.cu    # tests a specific file
set -uo pipefail
cd "$(dirname "$0")"

ENTRY_GLOB=(starter/*.cu)
SRC="${1:-${ENTRY_GLOB[0]}}"
[ -f "$SRC" ] || { echo "no such file: $SRC"; exit 1; }

export PATH=/usr/local/cuda/bin:$PATH
command -v nvcc >/dev/null || { echo "nvcc not found. Log in to the cluster first."; exit 1; }

HARNESS=""
for p in ../harness ../../_toolkit/agrade/harness ../../../_toolkit/agrade/harness; do
  [ -f "$p/atest.h" ] && HARNESS="$p" && break
done
[ -n "$HARNESS" ] || { echo "could not find atest.h"; exit 1; }

echo "==> compiling $SRC"
nvcc -O2 -std=c++17 -arch=sm_80 -Xcompiler "-Wall,-Wextra,-Werror" \
     -I"$HARNESS" -Iinclude "$SRC" tests/public.cu -o .selfcheck -lm || exit 1

# Never run compute on the login node: that is what the handbook tells you not
# to do, and the scheduler is right there.
if command -v srun >/dev/null; then
  echo "==> running via srun on a GPU node"
  srun --gres=gpu:1 --time=00:10:00 --mem=16G ./.selfcheck
else
  echo "==> running locally"
  ./.selfcheck
fi
rc=$?
rm -f .selfcheck
exit $rc
