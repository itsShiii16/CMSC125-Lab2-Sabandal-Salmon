#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "gantt.h"

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
    char process_input[1024];
    int use_inline_input;
    char mlfq_config_file[256];
    int quantum;
    int compare_mode;
} SchedulerConfig;

/* Global scheduler simulation state */
typedef struct {
    Process processes[MAX_PROCESSES];
    int process_count;
    int current_time;

    GanttEntry gantt_chart[MAX_GANTT_ENTRIES];
    int gantt_count;
} SchedulerState;

/* Scheduler implementations */
void schedule_fcfs(SchedulerState *state);
void schedule_sjf(SchedulerState *state);
void schedule_stcf(SchedulerState *state);
void schedule_rr(SchedulerState *state, int quantum);
void schedule_mlfq(SchedulerState *state, const char *config_file);

#endif