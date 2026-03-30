#include <stdio.h>
#include <string.h>

#include "../include/gantt.h"

void init_gantt(GanttEntry chart[], int *count)
{
    *count = 0;

    for (int i = 0; i < MAX_GANTT_ENTRIES; i++) {
        chart[i].pid[0] = '\0';
        chart[i].start_time = 0;
        chart[i].end_time = 0;
    }
}

void add_gantt_entry(GanttEntry chart[], int *count, const char *pid, int start_time, int end_time)
{
    if (*count >= MAX_GANTT_ENTRIES) {
        return;
    }

    /* Merge consecutive entries of the same process */
    if (*count > 0) {
        GanttEntry *last = &chart[*count - 1];

        if (strcmp(last->pid, pid) == 0 && last->end_time == start_time) {
            last->end_time = end_time;
            return;
        }
    }

    strcpy(chart[*count].pid, pid);
    chart[*count].start_time = start_time;
    chart[*count].end_time = end_time;
    (*count)++;
}

void print_gantt_chart(GanttEntry chart[], int count)
{
    printf("\nGantt Chart\n");
    printf("-----------------------------------------------------\n");

    if (count == 0) {
        printf("No execution recorded.\n");
        return;
    }

    /* Print process blocks */
    for (int i = 0; i < count; i++) {
        printf("| %s ", chart[i].pid);
    }
    printf("|\n");

    /* Print timeline */
    printf("%d", chart[0].start_time);
    for (int i = 0; i < count; i++) {
        printf("    %d", chart[i].end_time);
    }
    printf("\n");
}