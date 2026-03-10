#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

/* Supported scheduling algorithms */
typedef enum {
    ALG_NONE,
    ALG_FCFS,
    ALG_SJF,
    ALG_STCF,
    ALG_RR,
    ALG_MLFQ
} Algorithm;


/* Configuration parsed from CLI arguments */
typedef struct {
    Algorithm algorithm;
    char input_file[256];
    char mlfq_config_file[256];
    int quantum;
    int compare_mode;
} SchedulerConfig;


/* Global scheduler simulation state */
typedef struct {
    Process processes[MAX_PROCESSES];
    int process_count;
    int current_time;
} SchedulerState;


/* FCFS and sjf scheduler implementation */
void schedule_fcfs(SchedulerState *state);
void schedule_sjf(SchedulerState *state);

#endif