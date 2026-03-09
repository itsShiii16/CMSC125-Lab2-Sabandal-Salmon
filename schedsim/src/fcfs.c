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

        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;

        p->remaining_time = 0;
    }

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

    /* Calculate averages */
    double total_tt = 0;
    double total_wt = 0;
    double total_rt = 0;

    for (int i = 0; i < n; i++) {
        total_tt += processes[i].turnaround_time;
        total_wt += processes[i].waiting_time;
        total_rt += processes[i].response_time;
    }

    double avg_tt = total_tt / n;
    double avg_wt = total_wt / n;
    double avg_rt = total_rt / n;

    printf("\nAverage Turnaround Time: %.2f\n", avg_tt);
    printf("Average Waiting Time   : %.2f\n", avg_wt);
    printf("Average Response Time  : %.2f\n", avg_rt);
}