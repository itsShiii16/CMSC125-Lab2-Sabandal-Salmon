# CPU Scheduling Simulator Design
CMSC 125 - Operating Systems

## 1. Scope and Goals

This project is a modular CPU scheduling simulator written in C. It simulates process execution over discrete time, computes standard scheduling metrics, and prints ASCII Gantt charts.

Goals:
- Support multiple scheduling policies in one executable.
- Allow file-based and inline process inputs.
- Provide per-process and average metrics.
- Enable compare mode to run all algorithms on the same workload.

## 2. Implemented Algorithms

The simulator currently implements all required algorithms:
- FCFS (First-Come First-Serve, non-preemptive)
- SJF (Shortest Job First, non-preemptive)
- STCF (Shortest Time to Completion First, preemptive)
- RR (Round Robin, preemptive, configurable quantum)
- MLFQ (Multi-Level Feedback Queue, configurable queues/quantums/boost)

Each scheduler produces:
- Metrics table (AT, BT, ST, FT, TT, WT, RT)
- Average TT/WT/RT
- ASCII Gantt chart

## 3. Architecture

Main modules:
- Process loading/parsing: reads workload files or inline definitions.
- Argument/config parser: handles CLI options and validation.
- Scheduler implementations: one source file per algorithm.
- Metrics module: computes TT/WT/RT and averages.
- Gantt module: stores and prints execution segments.
- Checker module: validates schedule consistency.

Top-level flow:
1. Parse CLI args.
2. Validate config.
3. Load processes.
4. Run selected algorithm or compare mode.
5. Compute and print metrics/Gantt output.
6. Validate produced schedule.

## 4. Compare Mode Design

Compare mode runs FCFS, SJF, STCF, RR, and MLFQ against the same baseline workload in a single execution.

Implementation notes:
- A baseline copy of all processes is saved before the first run.
- Before each algorithm, process state is restored from baseline.
- Per-run output is printed normally by each scheduler.
- A final summary table prints average TT/WT/RT per algorithm.
- RR defaults to quantum=2 in compare mode if not explicitly provided.
- MLFQ defaults to tests/mlfq.txt in compare mode if not explicitly provided.

## 5. Repository Layout

```
schedsim/
├── Makefile
├── README.md
├── include/
├── src/
├── tests/
└── docs/
```

Key docs:
- docs/design.md: overall architecture and implementation status.
- docs/mlfq_design.md: MLFQ-specific policy and behavior.

## 6. Testing Strategy

Automated checks are provided in tests/test_suite.sh.

The suite covers:
- Build success
- FCFS run smoke test
- Compare mode run smoke test
- Argument validation for invalid RR quantum
- MLFQ config requirement enforcement in normal mode

Additional manual scenarios:
- Same-arrival workloads
- Idle-gap workloads
- Larger preemptive workloads for Gantt behavior

## 7. Known Limitations

- Single CPU only.
- No I/O blocking or multi-core modeling.
- ASCII console output only.
- Compare mode summary reports aggregate metrics only.

## 8. Summary

The simulator is complete for the target lab scope: all required schedulers are implemented, compare mode is operational, and documentation/testing artifacts are now aligned with the codebase.


