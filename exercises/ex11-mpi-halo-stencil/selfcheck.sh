#!/usr/bin/env bash
# Run the same three rank counts the grader uses.
#
#     ./selfcheck.sh                # tests starter/stencil.c
#     ./selfcheck.sh AIE23001.c     # tests a specific file
set -uo pipefail
cd "$(dirname "$0")"

SRC="${1:-starter/stencil.c}"
[ -f "$SRC" ] || SRC="stencil.c"
[ -f "$SRC" ] || { echo "no source file found"; exit 1; }

echo "==> compiling $SRC"
mpicc -O2 -std=c11 -Wall -Wextra -Werror -Iinclude "$SRC" -o .selfcheck -lm || exit 1

rc=0
for p in 1 4 7; do
  echo "==> $p rank(s)"
  if mpirun --oversubscribe -np "$p" ./.selfcheck 240 500 | tee /dev/stderr | grep -q "RESULT OK"; then
    echo "    ok"
  else
    echo "    FAILED at $p ranks"; rc=1
  fi
done
rm -f .selfcheck
exit $rc
