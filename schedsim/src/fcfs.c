#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"

/* Helper function for sorting by arrival time */
static int compare_arrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;

    if (p1->arrival_time == p2->arrival_time) {
        return strcmp(p1->pid, p2->pid);
    }

    return p1->arrival_time - p2->arrival_time;
}

/* FCFS Scheduler */
void schedule_fcfs(SchedulerState *state) {
    Process *processes = state->processes;
    int n = state->process_count;

    qsort(processes, n, sizeof(Process), compare_arrival);

    state->current_time = 0;

    printf("\nRunning FCFS Scheduler\n");
    printf("-----------------------------\n");

    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];

        if (state->current_time < p->arrival_time) {
            state->current_time = p->arrival_time;
        }

        p->start_time = state->current_time;
        state->current_time += p->burst_time;
        p->finish_time = state->current_time;
        p->remaining_time = 0;
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
}