#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/schedsim"

PASS=0
FAIL=0

pass() {
	printf "[PASS] %s\n" "$1"
	PASS=$((PASS + 1))
}

fail() {
	printf "[FAIL] %s\n" "$1"
	FAIL=$((FAIL + 1))
}

assert_contains() {
	local output="$1"
	local expected="$2"
	local label="$3"

	if [[ "$output" == *"$expected"* ]]; then
		pass "$label"
	else
		fail "$label"
		printf "  Expected to find: %s\n" "$expected"
	fi
}

assert_command_success() {
	local label="$1"
	shift

	if "$@" >/dev/null 2>&1; then
		pass "$label"
	else
		fail "$label"
	fi
}

assert_command_failure() {
	local label="$1"
	shift

	if "$@" >/dev/null 2>&1; then
		fail "$label"
	else
		pass "$label"
	fi
}

cd "$ROOT_DIR"

assert_command_success "Build succeeds" make clean all

fcfs_output="$($BIN --algorithm=FCFS --input=tests/workload1.txt 2>&1 || true)"
assert_contains "$fcfs_output" "Running FCFS Scheduler" "FCFS executes"
assert_contains "$fcfs_output" "Gantt Chart" "FCFS prints Gantt chart"
assert_contains "$fcfs_output" "Schedule validation PASSED" "FCFS validation passes"

compare_output="$($BIN --compare tests/workload1.txt 2>&1 || true)"
assert_contains "$compare_output" "Mode      : COMPARE" "Compare mode recognized"
assert_contains "$compare_output" "COMPARE RUN: FCFS" "Compare runs FCFS"
assert_contains "$compare_output" "COMPARE RUN: MLFQ" "Compare runs MLFQ"
assert_contains "$compare_output" "Compare Summary" "Compare summary printed"

assert_command_failure \
	"RR rejects non-numeric quantum" \
	"$BIN" --algorithm=RR --quantum=abc --input=tests/workload1.txt

mlfq_error_output="$($BIN --algorithm=MLFQ --input=tests/workload1.txt 2>&1 || true)"
assert_contains \
	"$mlfq_error_output" \
	"Error: MLFQ requires --mlfq=<config_file>." \
	"MLFQ requires config in normal mode"

printf "\nTotal Passed: %d\n" "$PASS"
printf "Total Failed: %d\n" "$FAIL"

if [[ $FAIL -ne 0 ]]; then
	exit 1
fi

printf "All tests passed.\n"

