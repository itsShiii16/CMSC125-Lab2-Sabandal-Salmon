#include <stdio.h>
#include <string.h>

#include "../include/process.h"
#include "../include/scheduler.h"

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

        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;

        p->remaining_time = 0;
        completed++;
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

    double total_tt = 0;
    double total_wt = 0;
    double total_rt = 0;

    for (int i = 0; i < n; i++) {
        total_tt += processes[i].turnaround_time;
        total_wt += processes[i].waiting_time;
        total_rt += processes[i].response_time;
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tt / n);
    printf("Average Waiting Time   : %.2f\n", total_wt / n);
    printf("Average Response Time  : %.2f\n", total_rt / n);
}