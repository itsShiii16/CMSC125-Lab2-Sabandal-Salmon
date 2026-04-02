#include <stdio.h>
#include <limits.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/gantt.h"

void schedule_rr(SchedulerState *state, int quantum) {
    Process *processes = state->processes;
    int n = state->process_count;

    state->current_time = 0;
    int completed = 0;

    init_gantt(state->gantt_chart, &state->gantt_count);

    printf("\nRunning Round Robin Scheduler (Round-Based)\n");
    printf("-----------------------------\n");

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
    }

    while (completed < n) {
        int round_start_time = state->current_time;

        int queue[MAX_PROCESSES];
        int q_size = 0;

        /* Snapshot ready processes at this round's start time. */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= round_start_time &&
                processes[i].remaining_time > 0) {
                queue[q_size++] = i;
            }
        }

        if (q_size == 0) {
            int next_arrival = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (processes[i].remaining_time > 0 &&
                    processes[i].arrival_time > state->current_time &&
                    processes[i].arrival_time < next_arrival) {
                    next_arrival = processes[i].arrival_time;
                }
            }

            if (next_arrival == INT_MAX) {
                break;
            }

            state->current_time = next_arrival;
            continue;
        }

        for (int q = 0; q < q_size; q++) {
            int i = queue[q];
            Process *p = &processes[i];

            if (p->remaining_time <= 0) continue;

            if (p->start_time == -1) {
                p->start_time = state->current_time;
            }

            int start = state->current_time;

            int time_slice = quantum;
            if (p->remaining_time < quantum) {
                time_slice = p->remaining_time;
            }

            p->remaining_time -= time_slice;
            state->current_time += time_slice;

            int end = state->current_time;

            add_gantt_entry(state->gantt_chart, &state->gantt_count, p->pid, start, end);

            if (p->remaining_time == 0) {
                completed++;
                p->finish_time = state->current_time;
            }
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
    print_gantt_chart(state->gantt_chart, state->gantt_count);
}