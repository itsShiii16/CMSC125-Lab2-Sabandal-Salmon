#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/process.h"

static void init_process(Process *p, const char *pid, int arrival, int burst) {
    strncpy(p->pid, pid, sizeof(p->pid) - 1);
    p->pid[sizeof(p->pid) - 1] = '\0';
    p->arrival_time = arrival;
    p->burst_time = burst;

    p->remaining_time = p->burst_time;

    p->start_time = -1;
    p->finish_time = -1;

    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->response_time = 0;
}

static char *trim_whitespace(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }

    if (*s == '\0') {
        return s;
    }

    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    return s;
}


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
            init_process(&p, p.pid, p.arrival_time, p.burst_time);

            processes[count++] = p;

            if (count >= MAX_PROCESSES) {
                break;
            }
        }
    }

    fclose(file);

    return count;
}


int parse_processes_from_string(const char *input, Process processes[]) {

    if (input == NULL || input[0] == '\0') {
        fprintf(stderr, "Error: empty inline process input.\n");
        return -1;
    }

    char buffer[1024];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    int count = 0;
    char *token = strtok(buffer, ";");

    while (token != NULL) {
        char *entry = trim_whitespace(token);

        if (entry[0] == '\0') {
            token = strtok(NULL, ";");
            continue;
        }

        Process p;
        int scanned = sscanf(entry, "%15s %d %d", p.pid, &p.arrival_time, &p.burst_time);

        if (scanned != 3) {
            fprintf(stderr, "Error: invalid process entry '%s'. Expected format: PID ARRIVAL BURST\n", entry);
            return -1;
        }

        init_process(&p, p.pid, p.arrival_time, p.burst_time);
        processes[count++] = p;

        if (count >= MAX_PROCESSES) {
            break;
        }

        token = strtok(NULL, ";");
    }

    if (count == 0) {
        fprintf(stderr, "Error: no valid processes found in inline input.\n");
        return -1;
    }

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