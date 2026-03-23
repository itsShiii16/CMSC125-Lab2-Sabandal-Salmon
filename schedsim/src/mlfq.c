#include <stdio.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/gantt.h"
#include "../include/mlfq.h"

void schedule_mlfq(SchedulerState *state, const char *config_file) {
    Process *processes = state->processes;
    int n = state->process_count;

    MLFQConfig config;

    if (!load_mlfq_config(config_file, &config)) {
        printf("Failed to load MLFQ config file.\n");
        return;
    }

    int completed = 0;
    state->current_time = 0;

    int queue_level[MAX_PROCESSES];
    int inserted[MAX_PROCESSES];

    /* Initialize Gantt chart */
    init_gantt(state->gantt_chart, &state->gantt_count);

    printf("\nRunning MLFQ Scheduler\n");
    printf("-----------------------------\n");

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        queue_level[i] = 0;
        inserted[i] = 0;
    }

    while (completed < n) {
        int selected = -1;
        int highest_priority = config.num_queues;

        /* Add newly arrived processes to highest priority queue */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0 &&
                !inserted[i]) {
                queue_level[i] = 0;
                inserted[i] = 1;
            }
        }

        /* Select process from highest non-empty queue */
        for (int level = 0; level < config.num_queues; level++) {
            for (int i = 0; i < n; i++) {
                if (inserted[i] &&
                    processes[i].remaining_time > 0 &&
                    queue_level[i] == level &&
                    processes[i].arrival_time <= state->current_time) {
                    selected = i;
                    highest_priority = level;
                    break;
                }
            }

            if (selected != -1) {
                break;
            }
        }

        /* CPU idle */
        if (selected == -1) {
            state->current_time++;
            continue;
        }

        Process *p = &processes[selected];

        if (p->start_time == -1) {
            p->start_time = state->current_time;
        }

        int start = state->current_time;
        int time_slice = config.quantum[highest_priority];

        if (p->remaining_time < time_slice) {
            time_slice = p->remaining_time;
        }

        p->remaining_time -= time_slice;
        state->current_time += time_slice;

        int end = state->current_time;

        /* Record Gantt entry */
        add_gantt_entry(state->gantt_chart, &state->gantt_count, p->pid, start, end);

        /* Add new arrivals during execution */
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0 &&
                !inserted[i]) {
                queue_level[i] = 0;
                inserted[i] = 1;
            }
        }

        if (p->remaining_time == 0) {
            completed++;
            p->finish_time = state->current_time;
        } else {
            /* Demote process if not yet finished */
            if (queue_level[selected] < config.num_queues - 1) {
                queue_level[selected]++;
            }
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
    print_gantt_chart(state->gantt_chart, state->gantt_count);
}