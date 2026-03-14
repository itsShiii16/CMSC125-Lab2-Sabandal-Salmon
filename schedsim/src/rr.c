#include <stdio.h>

#include "../include/process.h"
#include "../include/scheduler.h"

void schedule_rr(SchedulerState *state, int quantum) {

    Process *processes = state->processes;
    int n = state->process_count;

    int current_time = 0;
    int completed = 0;

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

            if (p->arrival_time <= current_time && p->remaining_time > 0) {

                executed = 1;

                /* first time CPU access */
                if (p->start_time == -1) {
                    p->start_time = current_time;
                }

                int time_slice = quantum;

                if (p->remaining_time < quantum) {
                    time_slice = p->remaining_time;
                }

                p->remaining_time -= time_slice;
                current_time += time_slice;

                if (p->remaining_time == 0) {

                    completed++;

                    p->finish_time = current_time;

                    p->turnaround_time =
                        p->finish_time - p->arrival_time;

                    p->waiting_time =
                        p->turnaround_time - p->burst_time;

                    p->response_time =
                        p->start_time - p->arrival_time;
                }
            }
        }

        /* CPU idle case */
        if (!executed) {
            current_time++;
        }
    }

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