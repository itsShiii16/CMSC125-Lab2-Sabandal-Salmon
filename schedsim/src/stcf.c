#include <stdio.h>
#include <limits.h>

#include "../include/process.h"
#include "../include/scheduler.h"


void schedule_stcf(SchedulerState *state) {

    Process *processes = state->processes;
    int n = state->process_count;

    int completed = 0;
    int current_time = 0;

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

            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                processes[i].remaining_time < shortest_time) {

                shortest = i;
                shortest_time = processes[i].remaining_time;
            }
        }

        /* if no process available */
        if (shortest == -1) {
            current_time++;
            continue;
        }

        Process *p = &processes[shortest];

        /* record first response time */
        if (p->start_time == -1) {
            p->start_time = current_time;
        }

        /* execute process for 1 time unit */
        p->remaining_time--;
        current_time++;

        /* if process finished */
        if (p->remaining_time == 0) {

            completed++;

            p->finish_time = current_time;

            p->turnaround_time = p->finish_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->response_time = p->start_time - p->arrival_time;
        }
    }


    /* print results */

    printf("\nResults\n");
    printf("-----------------------------------------------------\n");
    printf("PID\tAT\tBT\tST\tFT\tTT\tWT\tRT\n");

    float total_tt = 0;
    float total_wt = 0;
    float total_rt = 0;

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

        total_tt += p->turnaround_time;
        total_wt += p->waiting_time;
        total_rt += p->response_time;
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tt / n);
    printf("Average Waiting Time   : %.2f\n", total_wt / n);
    printf("Average Response Time  : %.2f\n", total_rt / n);
}