#include <stdio.h>
#include <limits.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/gantt.h"

void schedule_stcf(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->process_count;

    int completed = 0;
    state->current_time = 0;

    /* Initialize Gantt chart */
    init_gantt(state->gantt_chart, &state->gantt_count);

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

        /* CPU idle */
        if (shortest == -1) {
            state->current_time++;
            continue;
        }

        Process *p = &processes[shortest];

        /* record first CPU access */
        if (p->start_time == -1) {
            p->start_time = state->current_time;
        }

        int start = state->current_time;

        /* execute for 1 unit */
        p->remaining_time--;
        state->current_time++;

        int end = state->current_time;

        /* record Gantt entry */
        add_gantt_entry(state->gantt_chart, &state->gantt_count, p->pid, start, end);

        /* if process finished */
        if (p->remaining_time == 0) {
            completed++;
            p->finish_time = state->current_time;
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
    print_gantt_chart(state->gantt_chart, state->gantt_count);
}