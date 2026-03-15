#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 100

typedef struct {
    char pid[16];

    int arrival_time;
    int burst_time;
    int remaining_time;

    int start_time;
    int finish_time;

    int turnaround_time;
    int waiting_time;
    int response_time;

} Process;


/* Loads processes from a workload file */
int load_processes(const char *filename, Process processes[]);

/* Loads processes from a semicolon-separated inline string */
int parse_processes_from_string(const char *input, Process processes[]);

/* Prints processes for debugging / verification */
void print_processes(Process processes[], int count);

#endif