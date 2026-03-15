#include <stdio.h>
#include "../include/checker.h"

int validate_schedule(Process processes[], int count)
{
    int valid = 1;

    for (int i = 0; i < count; i++) {

        Process *p = &processes[i];

        if (p->start_time < p->arrival_time) {
            printf("ERROR: Process %s started before arrival\n", p->pid);
            valid = 0;
        }

        if (p->finish_time < p->start_time) {
            printf("ERROR: Process %s finished before it started\n", p->pid);
            valid = 0;
        }
    }

    return valid;
}