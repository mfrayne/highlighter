#!/usr/bin/env python3

import os
import re
import sys
import subprocess
from pathlib import Path


RED = "\033[0;31m"
GREEN = "\033[0;32m"
YELLOW = "\033[1;33m"
BLUE = "\033[0;34m"
NC = "\033[0m"


TOTAL_POINTS = 23.0
TIMEOUT_SECS = 5.0


def cleanup_before_test():
    # Ignore errors if paths do not exist
    for p in ["out.txt", "stdout.tmp", "stderr.tmp"]:
        try:
            os.remove(p)
        except FileNotFoundError:
            pass
        except PermissionError:
            pass


def read_tests_file(tests_path: Path):
    with tests_path.open("r", encoding="utf-8", errors="replace") as f:
        # Normalize CRLF; yield raw lines
        for raw in f:
            yield raw.rstrip("\n").rstrip("\r")


def first_pass_sections(lines):
    section_order = []
    section_percent = {}
    section_tests = {}

    current_section_id = None
    sec_count = 0

    sec_re = re.compile(r"^#\s*([0-9]+)\s+Section\b")

    for line in lines:
        m = sec_re.match(line)
        if m:
            current_section_id = f"sec_{sec_count}"
            sec_count += 1
            section_order.append(current_section_id)
            section_percent[current_section_id] = int(m.group(1))
            section_tests[current_section_id] = 0
            continue
        if not line or line.lstrip().startswith('#'):
            continue
        if current_section_id is None:
            current_section_id = f"sec_{sec_count}"
            sec_count += 1
            section_order.append(current_section_id)
            section_percent[current_section_id] = 100
            section_tests[current_section_id] = 0
        section_tests[current_section_id] += 1

    return section_order, section_percent, section_tests


def second_pass_run(lines, section_order, section_percent, section_tests):
    total_tests = 0
    passed_tests = 0
    failed_tests = 0
    total_points_earned = 0.0

    sec_re = re.compile(r"^#\s*([0-9]+)\s+Section\b")
    sections_seen = 0
    current_section_id = None
    per_test_points = 0.0

    for line in lines:
        if sec_re.match(line):
            # Move to next section
            current_section_id = section_order[sections_seen] if sections_seen < len(section_order) else None
            sections_seen += 1
            if current_section_id is not None:
                sec_percent = section_percent.get(current_section_id, 0)
                sec_tests = section_tests.get(current_section_id, 0)
                section_points = (sec_percent / 100.0) * TOTAL_POINTS
                per_test_points = (section_points / sec_tests) if sec_tests > 0 else 0.0
                print()
                print(f"---- Section ({sec_percent}% of {int(TOTAL_POINTS)} pts) - {sec_tests} test(s) ----")
            else:
                per_test_points = 0.0
            continue

        if not line or line.lstrip().startswith('#'):
            continue

        total_tests += 1
        print(f"{BLUE}[Test {total_tests}]{NC} {line}")
        cleanup_before_test()

        # Execute line with timeout
        timed_out = False
        try:
            proc = subprocess.run(line, shell=True, timeout=TIMEOUT_SECS)
            exit_code = proc.returncode
        except subprocess.TimeoutExpired:
            timed_out = True
            exit_code = 1
        except Exception:
            exit_code = 1

        if exit_code == 0:
            passed_tests += 1
            total_points_earned += per_test_points
            print(f"  {GREEN}[PASS]{NC} (+{per_test_points:.3f} pts)")
        else:
            failed_tests += 1
            if timed_out:
                print(f"  {RED}[FAIL]{NC} (timeout after {TIMEOUT_SECS:.0f}s)")
            else:
                print(f"  {RED}[FAIL]{NC} (exit {exit_code})")

    return total_tests, passed_tests, failed_tests, total_points_earned


def main() -> int:
    here = Path(__file__).resolve().parent
    os.chdir(here)

    print("================================================")
    print("  Testing Highlighter Assignment")
    print("================================================")
    print()

    if not (here / "highlighter").exists():
        print(f"{RED}[ERROR]{NC} highlighter executable not found!")
        print("Please compile the project first with: gcc highlighter.c -o highlighter")
        return 1

    tests_path = here / "tests" / "tests.txt"
    if not tests_path.exists():
        print(f"{RED}[ERROR]{NC} tests/tests.txt not found!")
        return 1

    # Read file once into memory for two passes
    all_lines = list(read_tests_file(tests_path))

    section_order, section_percent, section_tests = first_pass_sections(all_lines)

    total_tests, passed_tests, failed_tests, total_points_earned = second_pass_run(
        all_lines, section_order, section_percent, section_tests
    )

    print()
    print("================================================")
    print("  Summary")
    print("================================================")
    print(f"Total tests: {total_tests}")
    print(f"{GREEN}Tests passed: {passed_tests}{NC}")
    print(f"{RED}Tests failed: {failed_tests}{NC}")
    print()
    print(f"points for correctness: {total_points_earned:.3f}/{TOTAL_POINTS}, the other 2 points are for comments and git commits, please read the document for more details.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
