#include <stdio.h>
#include "../include/checker.h"

int validate_schedule(Process processes[], int count)
{
    int valid = 1;

    for (int i = 0; i < count; i++) {

        Process *p = &processes[i];

        if (p->start_time < p->arrival_time) {
            fprintf(stderr, "ERROR: Process %s started before arrival\n", p->pid);
            valid = 0;
        }

        if (p->finish_time < p->start_time) {
            fprintf(stderr, "ERROR: Process %s finished before it started\n", p->pid);
            valid = 0;
        }

        if (p->turnaround_time != p->finish_time - p->arrival_time) {
            fprintf(stderr, "ERROR: Turnaround mismatch for %s\n", p->pid);
            valid = 0;
        }

        if (p->waiting_time != p->turnaround_time - p->burst_time) {
            fprintf(stderr, "ERROR: Waiting time mismatch for %s\n", p->pid);
            valid = 0;
        }

        if (p->response_time != p->start_time - p->arrival_time) {
            fprintf(stderr, "ERROR: Response time mismatch for %s\n", p->pid);
            valid = 0;
        }
    }

    if (valid) {
        printf("\nSchedule validation PASSED\n");
    } else {
        fprintf(stderr, "\nSchedule validation FAILED\n");
    }

    return valid;
}