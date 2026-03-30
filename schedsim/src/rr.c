#include <stdio.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/gantt.h"

void schedule_rr(SchedulerState *state, int quantum) {
    Process *processes = state->processes;
    int n = state->process_count;

    state->current_time = 0;
    int completed = 0;

    /* Initialize Gantt chart */
    init_gantt(state->gantt_chart, &state->gantt_count);

    printf("\nRunning Round Robin Scheduler\n");
    printf("-----------------------------\n");

    /* initialize remaining times */
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
    }

    while (completed < n) {
        int executed = 0;

        for (int i = 0; i < n; i++) {
            Process *p = &processes[i];

            if (p->arrival_time <= state->current_time && p->remaining_time > 0) {
                executed = 1;

                /* first time CPU access */
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

                /* Record Gantt entry */
                add_gantt_entry(state->gantt_chart, &state->gantt_count, p->pid, start, end);

                if (p->remaining_time == 0) {
                    completed++;
                    p->finish_time = state->current_time;
                }
            }
        }

        /* CPU idle case */
        if (!executed) {
            state->current_time++;
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
    print_gantt_chart(state->gantt_chart, state->gantt_count);
}