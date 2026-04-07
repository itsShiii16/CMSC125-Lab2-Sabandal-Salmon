# MLFQ Design Notes

## 1. Objective

The Multi-Level Feedback Queue (MLFQ) scheduler is designed to balance responsiveness and fairness.

- Short/interactive jobs should get CPU quickly.
- Long CPU-bound jobs should still make progress.
- Starvation is mitigated using periodic priority boost.

## 2. Configuration Model

MLFQ is configured through a text file.

Required keys:
- queues=<N>
- q0=<quantum>
- q1=<quantum>
- ...
- q(N-1)=<quantum>

Optional key:
- boost=<time>

Example:

```
queues=3
q0=2
q1=4
q2=8
boost=50
```

## 3. Runtime State

Per-process scheduler state:
- remaining_time
- queue_level
- start_time/finish_time
- allotment_used at current level

Global scheduler state:
- current_time
- completed count
- last_boost_time

## 4. Scheduling Rules

1. New arrivals enter highest priority queue (level 0).
2. Scheduler selects runnable process from highest non-empty priority level.
3. Process executes for up to that level's quantum.
4. allotment_used increases by executed slice.
5. If process completes, record finish time.
6. If process does not complete and uses full level allotment, demote by one level.
7. On boost interval, all unfinished processes are promoted to level 0 and allotment counters reset.

Current implementation uses per-level allotment equal to level quantum.

## 5. Tie-Breaking and Determinism

When multiple runnable processes are in the same queue level, selection follows process array order (stable by load order unless a scheduler run reorders the array).

## 6. Complexity

For each scheduling decision, the implementation scans levels and processes:
- Time complexity per decision: O(N * Q) in worst case
- Memory overhead: O(N + Q)

Where:
- N = number of processes
- Q = number of queues

## 7. Validation Focus

Important correctness checks:
- New arrivals always start at highest priority.
- Demotion occurs only after allotment exhaustion.
- Boost actually resets queue levels for unfinished processes.
- TT/WT/RT match finish/start timestamps.
- Gantt entries reflect execution segments.

