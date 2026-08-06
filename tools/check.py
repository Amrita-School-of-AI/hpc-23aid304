#!/usr/bin/env python3
"""
Run the public tests for one or more exercises and print a results table.

This is the student-facing checker. It ships inside the course repository and is
what the GitHub Actions workflow runs on every push, so you get the same
pass/fail table in the Actions tab that you get on your own machine.

    tools/check.py                       # every exercise that has a submission
    tools/check.py ex06-openmp-reduction # just one
    tools/check.py --list                # what exists

It only ever runs the PUBLIC tests. The real grade also includes hidden tests, a
determinism check and, for some exercises, a speedup measurement, so a clean run
here is necessary but not sufficient.
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXERCISES = ROOT / "exercises"
SUBMISSIONS = ROOT / "submissions"

EXT = {".c": "c", ".cpp": "cpp", ".cu": "cuda", ".py": "python"}


def exercise_dirs() -> list[Path]:
    return sorted(d for d in EXERCISES.glob("*/")
                  if d.is_dir() and d.name != "harness")


def submission_for(ex: Path) -> Path | None:
    """The file the student put in submissions/<exercise>/, if any.

    Falls back to the starter so that a fresh clone still produces a table
    rather than an empty report.
    """
    d = SUBMISSIONS / ex.name
    if d.is_dir():
        files = sorted(p for p in d.iterdir()
                       if p.is_file() and p.suffix in EXT and not p.name.startswith("."))
        if files:
            return files[0]
    starters = sorted((ex / "starter").glob("*"))
    return starters[0] if starters else None


def language(path: Path) -> str:
    return EXT.get(path.suffix, "c")


def run(ex: Path, src: Path) -> tuple[str, int, int, str]:
    """Returns (status, passed, total, detail)."""
    lang = language(src)

    if lang == "cuda" and not shutil.which("nvcc"):
        return ("skipped", 0, 0,
                "needs a GPU and the CUDA toolkit; run this one on the cluster "
                "with `./selfcheck.sh`")
    if lang == "c" and (ex / "include").is_dir():
        pass

    script = ex / "selfcheck.sh"
    if not script.exists():
        return ("error", 0, 0, "this exercise has no selfcheck.sh")

    env = dict(os.environ)
    env.setdefault("OMP_NUM_THREADS", "2")
    try:
        p = subprocess.run(["bash", str(script), str(src.resolve())],
                           capture_output=True, text=True, timeout=900, env=env)
    except subprocess.TimeoutExpired:
        return ("error", 0, 0, "timed out after 15 minutes")

    out = p.stdout + p.stderr
    passed = len(re.findall(r"^PASS ", out, re.M))
    failed = len(re.findall(r"^FAIL ", out, re.M))
    total = passed + failed

    if total == 0:
        # Whole-program exercises (MPI) print their own verdict rather than
        # per-case PASS/FAIL lines.
        verdicts = re.findall(r"^(RESULT OK|RESULT MISMATCH|ALL OK|FAILURES: \d+)",
                              out, re.M)
        if verdicts and all(v in ("RESULT OK", "ALL OK") for v in verdicts):
            return ("pass", len(verdicts), len(verdicts), "")
        if verdicts:
            bad = sum(1 for v in verdicts if v not in ("RESULT OK", "ALL OK"))
            # It built and ran; it just gave the wrong answer. That is a failed
            # test, not a broken build, and saying so matters to the student.
            return ("fail", len(verdicts) - bad, len(verdicts),
                    "ran, but the answer was wrong")
        first = next((l for l in out.splitlines()
                      if re.search(r"\berror\b", l, re.I)), "")
        return ("error", 0, 0, first[:160] or "did not build")

    detail = "" if failed == 0 else "; ".join(
        re.findall(r"^FAIL (\S+)", out, re.M)[:4])
    return ("pass" if failed == 0 else "fail", passed, total, detail)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("exercise", nargs="*", help="exercise folder name; default all")
    ap.add_argument("--list", action="store_true")
    a = ap.parse_args()

    dirs = exercise_dirs()
    if a.list:
        for d in dirs:
            sub = submission_for(d)
            where = "submission" if (sub and SUBMISSIONS in sub.parents) else "starter"
            print(f"  {d.name:<28} {where}")
        return

    if a.exercise:
        want = set(a.exercise)
        dirs = [d for d in dirs if d.name in want]
        if not dirs:
            sys.exit(f"no such exercise; try --list")

    rows, any_fail = [], False
    for d in dirs:
        src = submission_for(d)
        if src is None:
            continue
        mine = SUBMISSIONS in src.parents
        status, passed, total, detail = run(d, src)
        if status == "fail" or status == "error":
            if mine:
                any_fail = True
        rows.append((d.name, src.name, mine, status, passed, total, detail))

    icon = {"pass": "PASS", "fail": "FAIL", "skipped": "SKIP", "error": "ERR "}
    print()
    print(f"{'exercise':<28} {'file':<18} {'':<5} {'public tests':<14} notes")
    print("-" * 96)
    for name, fn, mine, status, p, t, detail in rows:
        score = f"{p}/{t}" if t else "-"
        tag = "" if mine else "(starter)"
        print(f"{name:<28} {fn:<18} {icon[status]:<5} {score:<14} {detail} {tag}".rstrip())

    submitted = [r for r in rows if r[2]]
    print()
    print(f"{len(submitted)} submission(s) checked, "
          f"{sum(1 for r in submitted if r[3] == 'pass')} passing")

    summary = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary:
        with open(summary, "a", encoding="utf-8") as fh:
            fh.write("## Public test results\n\n")
            fh.write("| Exercise | File | Result | Public tests | Notes |\n")
            fh.write("|---|---|---|---|---|\n")
            mark = {"pass": "PASS", "fail": "FAIL",
                    "skipped": "skipped", "error": "error"}
            for name, fn, mine, status, p, t, detail in rows:
                score = f"{p}/{t}" if t else "-"
                note = detail or ("your submission" if mine else "starter, not yet submitted")
                fh.write(f"| `{name}` | `{fn}` | **{mark[status]}** | {score} | {note} |\n")
            fh.write(
                "\n> These are the **public** tests only. Your final mark also "
                "includes hidden tests, a determinism check across thread counts "
                "and, for some exercises, a speedup measurement. A clean run here "
                "is necessary but not sufficient.\n")

    sys.exit(1 if any_fail else 0)


if __name__ == "__main__":
    main()
