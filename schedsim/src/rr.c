#include <stdio.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/gantt.h"

void schedule_rr(SchedulerState *state, int quantum) {
    Process *processes = state->processes;
    int n = state->process_count;

    state->current_time = 0;
    int completed = 0;

    /* Circular ready queue */
    int queue[MAX_PROCESSES];
    int front = 0;
    int rear = 0;
    int size = 0;

    int enqueued[MAX_PROCESSES];

    init_gantt(state->gantt_chart, &state->gantt_count);

    printf("\nRunning Round Robin Scheduler\n");
    printf("-----------------------------\n");

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
        processes[i].finish_time = 0;
        enqueued[i] = 0;
    }

    while (completed < n) {

        /* Enqueue newly arrived processes in arrival order */
        for (int i = 0; i < n; i++) {
            if (!enqueued[i] &&
                processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0) {

                queue[rear] = i;
                rear = (rear + 1) % MAX_PROCESSES;
                size++;
                enqueued[i] = 1;
            }
        }

        /* If queue is empty, jump to next arrival */
        if (size == 0) {
            int next_arrival = -1;

            for (int i = 0; i < n; i++) {
                if (processes[i].remaining_time > 0) {
                    if (next_arrival == -1 || processes[i].arrival_time < next_arrival) {
                        next_arrival = processes[i].arrival_time;
                    }
                }
            }

            if (next_arrival == -1) {
                break;
            }

            state->current_time = next_arrival;
            continue;
        }

        /* Dequeue next process */
        int index = queue[front];
        front = (front + 1) % MAX_PROCESSES;
        size--;

        Process *p = &processes[index];

        if (p->start_time == -1) {
            p->start_time = state->current_time;
        }

        int start = state->current_time;

        int time_slice = quantum;
        if (p->remaining_time < time_slice) {
            time_slice = p->remaining_time;
        }

        p->remaining_time -= time_slice;
        state->current_time += time_slice;

        int end = state->current_time;

        add_gantt_entry(state->gantt_chart,
                        &state->gantt_count,
                        p->pid,
                        start,
                        end);

        /* Enqueue any new arrivals that appeared during execution */
        for (int i = 0; i < n; i++) {
            if (!enqueued[i] &&
                processes[i].arrival_time <= state->current_time &&
                processes[i].remaining_time > 0) {

                queue[rear] = i;
                rear = (rear + 1) % MAX_PROCESSES;
                size++;
                enqueued[i] = 1;
            }
        }

        if (p->remaining_time == 0) {
            completed++;
            p->finish_time = state->current_time;
        } else {
            /* Rotate unfinished process to the back */
            queue[rear] = index;
            rear = (rear + 1) % MAX_PROCESSES;
            size++;
        }
    }

    compute_metrics(processes, n);
    print_results(processes, n);
    print_averages(processes, n);
    print_gantt_chart(state->gantt_chart, state->gantt_count);
}