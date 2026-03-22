#include <stdio.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"

#define Q0_QUANTUM 2
#define Q1_QUANTUM 4
#define Q2_QUANTUM 100000

void schedule_mlfq(SchedulerState *state, const char *config_file) {
    (void)config_file;

    Process *processes = state->processes;
    int n = state->process_count;

    int completed = 0;
    state->current_time = 0;

    int queue_level[MAX_PROCESSES];
    int inserted[MAX_PROCESSES];

    printf("\nRunning MLFQ Scheduler\n");
    printf("-----------------------------\n");

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        queue_level[i] = 0;
        inserted[i] = 0;
    }

    while (completed < n) {
        int selected = -1;
        int highest_priority = 3;

        /* Add newly arrived processes to highest priority queue */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0 &&
                !inserted[i]) {
                queue_level[i] = 0;
                inserted[i] = 1;
            }
        }

        /* Select process from highest non-empty queue */
        for (int level = 0; level < 3; level++) {
            for (int i = 0; i < n; i++) {
                if (inserted[i] &&
                    processes[i].remaining_time > 0 &&
                    queue_level[i] == level &&
                    processes[i].arrival_time <= state->current_time) {
                    selected = i;
                    highest_priority = level;
                    break;
                }
            }
            if (selected != -1) {
                break;
            }
        }

        /* CPU idle */
        if (selected == -1) {
            state->current_time++;
            continue;
        }

        Process *p = &processes[selected];

        if (p->start_time == -1) {
            p->start_time = state->current_time;
        }

        int time_slice;

        if (highest_priority == 0) {
            time_slice = Q0_QUANTUM;
        } else if (highest_priority == 1) {
            time_slice = Q1_QUANTUM;
        } else {
            time_slice = Q2_QUANTUM;
        }

        if (p->remaining_time < time_slice) {
            time_slice = p->remaining_time;
        }

        p->remaining_time -= time_slice;
        state->current_time += time_slice;

        /* Add any newly arrived processes during execution */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0 &&
                !inserted[i]) {
                queue_level[i] = 0;
                inserted[i] = 1;
            }
        }

        if (p->remaining_time == 0) {
            completed++;
            p->finish_time = state->current_time;
        } else {
            /* demote process if not yet finished */
            if (queue_level[selected] < 2) {
                queue_level[selected]++;
            }
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
}