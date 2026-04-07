#include <stdio.h>
#include <string.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/checker.h"
#include "../include/parser.h"

typedef struct {
    const char *name;
    double avg_tt;
    double avg_wt;
    double avg_rt;
    int ran;
} CompareResult;

static void restore_processes(SchedulerState *state, const Process baseline[], int count) {
    memcpy(state->processes, baseline, sizeof(Process) * count);
    state->process_count = count;
    state->current_time = 0;
    state->gantt_count = 0;
}

static void collect_averages(Process processes[], int count, CompareResult *result) {
    double total_tt = 0.0;
    double total_wt = 0.0;
    double total_rt = 0.0;

    for (int i = 0; i < count; i++) {
        total_tt += processes[i].turnaround_time;
        total_wt += processes[i].waiting_time;
        total_rt += processes[i].response_time;
    }

    result->avg_tt = total_tt / count;
    result->avg_wt = total_wt / count;
    result->avg_rt = total_rt / count;
    result->ran = 1;
}

static int run_algorithm(SchedulerState *state,
                         Algorithm algorithm,
                         int rr_quantum,
                         const char *mlfq_config_file) {
    if (algorithm == ALG_FCFS) {
        schedule_fcfs(state);
        return 1;
    }

    if (algorithm == ALG_SJF) {
        schedule_sjf(state);
        return 1;
    }

    if (algorithm == ALG_STCF) {
        schedule_stcf(state);
        return 1;
    }

    if (algorithm == ALG_RR) {
        schedule_rr(state, rr_quantum);
        return 1;
    }

    if (algorithm == ALG_MLFQ) {
        FILE *file = fopen(mlfq_config_file, "r");

        if (file == NULL) {
            fprintf(stderr,
                    "Skipping MLFQ in compare mode: unable to open config '%s'.\n",
                    mlfq_config_file);
            return 0;
        }

        fclose(file);
        schedule_mlfq(state, mlfq_config_file);
        return 1;
    }

    return 0;
}

static void run_compare_mode(SchedulerState *state, const SchedulerConfig *config) {
    Process baseline[MAX_PROCESSES];
    int count = state->process_count;

    CompareResult results[] = {
        {"FCFS", 0.0, 0.0, 0.0, 0},
        {"SJF", 0.0, 0.0, 0.0, 0},
        {"STCF", 0.0, 0.0, 0.0, 0},
        {"RR", 0.0, 0.0, 0.0, 0},
        {"MLFQ", 0.0, 0.0, 0.0, 0}
    };

    Algorithm algorithms[] = {ALG_FCFS, ALG_SJF, ALG_STCF, ALG_RR, ALG_MLFQ};

    int rr_quantum = config->quantum > 0 ? config->quantum : 2;
    const char *mlfq_file = config->mlfq_config_file[0] != '\0'
        ? config->mlfq_config_file
        : "tests/mlfq.txt";

    memcpy(baseline, state->processes, sizeof(Process) * count);

    if (config->quantum <= 0) {
        printf("Compare mode note: RR quantum not provided, using default quantum=2.\n");
    }

    if (config->mlfq_config_file[0] == '\0') {
        printf("Compare mode note: MLFQ config not provided, using %s.\n", mlfq_file);
    }

    for (int i = 0; i < 5; i++) {
        printf("\n========================================\n");
        printf("COMPARE RUN: %s\n", results[i].name);
        printf("========================================\n");

        restore_processes(state, baseline, count);

        if (!run_algorithm(state, algorithms[i], rr_quantum, mlfq_file)) {
            continue;
        }

        validate_schedule(state->processes, state->process_count);
        collect_averages(state->processes, state->process_count, &results[i]);
    }

    printf("\nCompare Summary\n");
    printf("-----------------------------------------------------\n");
    printf("Algorithm\tAvg TT\tAvg WT\tAvg RT\n");

    for (int i = 0; i < 5; i++) {
        if (results[i].ran) {
            printf("%s\t\t%.2f\t%.2f\t%.2f\n",
                   results[i].name,
                   results[i].avg_tt,
                   results[i].avg_wt,
                   results[i].avg_rt);
        } else {
            printf("%s\t\tSKIPPED\n", results[i].name);
        }
    }
}

int main(int argc, char *argv[]) {
    SchedulerConfig config;
    SchedulerState state = {0};

    if (!parse_args(argc, argv, &config)) {
        print_usage(argv[0]);
        return 1;
    }

    if (!validate_config(&config)) {
        print_usage(argv[0]);
        return 1;
    }

    state.current_time = 0;

    if (config.use_inline_input) {
        state.process_count = parse_processes_from_string(config.process_input, state.processes);
    } else {
        state.process_count = load_processes(config.input_file, state.processes);
    }

    if (state.process_count < 0) {
        return 1;
    }

    print_config(&config);
    print_processes(state.processes, state.process_count);

    if (config.compare_mode) {
        run_compare_mode(&state, &config);
        return 0;
    }

    if (config.algorithm == ALG_FCFS) {
        schedule_fcfs(&state);
    } else if (config.algorithm == ALG_SJF) {
        schedule_sjf(&state);
    } else if (config.algorithm == ALG_STCF) {
        schedule_stcf(&state);
    } else if (config.algorithm == ALG_RR) {
        schedule_rr(&state, config.quantum);
    } else if (config.algorithm == ALG_MLFQ) {
        schedule_mlfq(&state, config.mlfq_config_file);
    } else {
        fprintf(stderr, "Error: selected algorithm is not implemented yet.\n");
    }

    validate_schedule(state.processes, state.process_count);

    return 0;
}