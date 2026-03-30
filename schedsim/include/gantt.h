#ifndef GANTT_H
#define GANTT_H

#define MAX_GANTT_ENTRIES 1000

typedef struct {
    char pid[16];
    int start_time;
    int end_time;
} GanttEntry;

/* Initialize Gantt chart storage */
void init_gantt(GanttEntry chart[], int *count);

/* Add an execution segment */
void add_gantt_entry(GanttEntry chart[], int *count, const char *pid, int start_time, int end_time);

/* Print Gantt chart */
void print_gantt_chart(GanttEntry chart[], int count);

#endif