#include "../include/utils.h"

void reset_processes(Process processes[], int count)
{
    for (int i = 0; i < count; i++) {

        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].finish_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = 0;
    }
}

int all_processes_completed(Process processes[], int count)
{
    for (int i = 0; i < count; i++) {
        if (processes[i].remaining_time > 0) {
            return 0;
        }
    }
    return 1;
}