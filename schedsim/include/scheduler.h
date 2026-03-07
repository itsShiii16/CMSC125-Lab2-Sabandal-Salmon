#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef enum {
    ALG_NONE,
    ALG_FCFS,
    ALG_SJF,
    ALG_STCF,
    ALG_RR,
    ALG_MLFQ
} Algorithm;

typedef struct {
    Algorithm algorithm;
    char input_file[256];
    char mlfq_config_file[256];
    int quantum;
    int compare_mode;
} SchedulerConfig;

typedef struct {
    Process processes[MAX_PROCESSES];
    int process_count;
    int current_time;
} SchedulerState;

#endif