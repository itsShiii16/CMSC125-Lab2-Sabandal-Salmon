#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/process.h"


int load_processes(const char *filename, Process processes[]) {

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {

        /* Skip comment lines */
        if (line[0] == '#') {
            continue;
        }

        Process p;

        if (sscanf(line, "%s %d %d",
                   p.pid,
                   &p.arrival_time,
                   &p.burst_time) == 3) {

            p.remaining_time = p.burst_time;

            p.start_time = -1;
            p.finish_time = -1;

            p.turnaround_time = 0;
            p.waiting_time = 0;
            p.response_time = 0;

            processes[count++] = p;

            if (count >= MAX_PROCESSES) {
                break;
            }
        }
    }

    fclose(file);

    return count;
}


void print_processes(Process processes[], int count) {

    printf("\nLoaded Processes\n");
    printf("--------------------------\n");
    printf("PID\tAT\tBT\n");

    for (int i = 0; i < count; i++) {
        printf("%s\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time);
    }
}