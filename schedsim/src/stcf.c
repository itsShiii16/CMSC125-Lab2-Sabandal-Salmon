#include <stdio.h>
#include <limits.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"

void schedule_stcf(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->process_count;

    int completed = 0;
    state->current_time = 0;

    printf("\nRunning STCF Scheduler\n");
    printf("-----------------------------\n");

    /* initialize remaining times */
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
    }

    while (completed < n) {
        int shortest = -1;
        int shortest_time = INT_MAX;

        /* find process with shortest remaining time */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0 &&
                processes[i].remaining_time < shortest_time) {

                shortest = i;
                shortest_time = processes[i].remaining_time;
            }
        }

        /* if no process available */
        if (shortest == -1) {
            state->current_time++;
            continue;
        }

        Process *p = &processes[shortest];

        /* record first CPU access */
        if (p->start_time == -1) {
            p->start_time = state->current_time;
        }

        /* execute process for 1 time unit */
        p->remaining_time--;
        state->current_time++;

        /* if process finished */
        if (p->remaining_time == 0) {
            completed++;
            p->finish_time = state->current_time;
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
}