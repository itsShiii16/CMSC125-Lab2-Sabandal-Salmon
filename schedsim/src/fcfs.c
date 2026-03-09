#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/process.h"
#include "../include/scheduler.h"


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

    /* Sort processes by arrival time */
    qsort(processes, n, sizeof(Process), compare_arrival);

    state->current_time = 0;

    printf("\nRunning FCFS Scheduler\n");
    printf("-----------------------------\n");

    for (int i = 0; i < n; i++) {

        Process *p = &processes[i];

        /* If CPU is idle, jump to arrival time */
        if (state->current_time < p->arrival_time) {
            state->current_time = p->arrival_time;
        }

        /* Start time */
        p->start_time = state->current_time;

        /* Execute process */
        state->current_time += p->burst_time;

        /* Finish time */
        p->finish_time = state->current_time;

        /* Metrics */
        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;

        /* Process fully executed */
        p->remaining_time = 0;
    }


    /* Print results */
    printf("\nResults\n");
    printf("-----------------------------------------------------\n");
    printf("PID\tAT\tBT\tST\tFT\tTT\tWT\tRT\n");

    for (int i = 0; i < n; i++) {

        Process *p = &processes[i];

        printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p->pid,
               p->arrival_time,
               p->burst_time,
               p->start_time,
               p->finish_time,
               p->turnaround_time,
               p->waiting_time,
               p->response_time);
    }

}