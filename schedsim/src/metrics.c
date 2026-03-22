#include <stdio.h>

#include "../include/metrics.h"

void compute_metrics(Process processes[], int count)
{
    for (int i = 0; i < count; i++) {
        processes[i].turnaround_time =
            processes[i].finish_time - processes[i].arrival_time;

        processes[i].waiting_time =
            processes[i].turnaround_time - processes[i].burst_time;

        processes[i].response_time =
            processes[i].start_time - processes[i].arrival_time;
    }
}

void print_results(Process processes[], int count)
{
    printf("\nResults\n");
    printf("-----------------------------------------------------\n");
    printf("PID\tAT\tBT\tST\tFT\tTT\tWT\tRT\n");

    for (int i = 0; i < count; i++) {
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

void print_averages(Process processes[], int count)
{
    double total_tt = 0;
    double total_wt = 0;
    double total_rt = 0;

    for (int i = 0; i < count; i++) {
        total_tt += processes[i].turnaround_time;
        total_wt += processes[i].waiting_time;
        total_rt += processes[i].response_time;
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tt / count);
    printf("Average Waiting Time   : %.2f\n", total_wt / count);
    printf("Average Response Time  : %.2f\n", total_rt / count);
}