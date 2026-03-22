#include <stdio.h>
#include <string.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"

void schedule_sjf(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->process_count;
    int completed = 0;

    state->current_time = 0;

    printf("\nRunning SJF Scheduler\n");
    printf("-----------------------------\n");

    while (completed < n) {
        int shortest_index = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0 &&
                processes[i].arrival_time <= state->current_time) {

                if (shortest_index == -1 ||
                    processes[i].burst_time < processes[shortest_index].burst_time ||
                    (processes[i].burst_time == processes[shortest_index].burst_time &&
                     strcmp(processes[i].pid, processes[shortest_index].pid) < 0)) {
                    shortest_index = i;
                }
            }
        }

        if (shortest_index == -1) {
            state->current_time++;
            continue;
        }

        Process *p = &processes[shortest_index];

        p->start_time = state->current_time;
        state->current_time += p->burst_time;
        p->finish_time = state->current_time;

        p->remaining_time = 0;
        completed++;
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
}