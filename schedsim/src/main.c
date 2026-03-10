#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/process.h"
#include "../include/scheduler.h"

static void print_usage(const char *program_name) {
    printf("Usage:\n");
    printf("  %s --algorithm=<FCFS|SJF|STCF|RR|MLFQ> --input=<file>\n", program_name);
    printf("  %s --algorithm=RR --quantum=<time> --input=<file>\n", program_name);
    printf("  %s --algorithm=MLFQ --mlfq=<config_file> --input=<file>\n", program_name);
    printf("  %s --compare <file>\n", program_name);
}

static void init_config(SchedulerConfig *config) {
    config->algorithm = ALG_NONE;
    config->input_file[0] = '\0';
    config->mlfq_config_file[0] = '\0';
    config->quantum = -1;
    config->compare_mode = 0;
}

static Algorithm parse_algorithm(const char *value) {
    if (strcmp(value, "FCFS") == 0) return ALG_FCFS;
    if (strcmp(value, "SJF") == 0) return ALG_SJF;
    if (strcmp(value, "STCF") == 0) return ALG_STCF;
    if (strcmp(value, "RR") == 0) return ALG_RR;
    if (strcmp(value, "MLFQ") == 0) return ALG_MLFQ;
    return ALG_NONE;
}

static const char *algorithm_to_string(Algorithm algorithm) {
    switch (algorithm) {
        case ALG_FCFS: return "FCFS";
        case ALG_SJF:  return "SJF";
        case ALG_STCF: return "STCF";
        case ALG_RR:   return "RR";
        case ALG_MLFQ: return "MLFQ";
        default:       return "NONE";
    }
}

static int parse_args(int argc, char *argv[], SchedulerConfig *config) {
    init_config(config);

    if (argc < 2) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            const char *value = argv[i] + 12;
            config->algorithm = parse_algorithm(value);
        } else if (strncmp(argv[i], "--input=", 8) == 0) {
            const char *value = argv[i] + 8;
            strncpy(config->input_file, value, sizeof(config->input_file) - 1);
            config->input_file[sizeof(config->input_file) - 1] = '\0';
        } else if (strncmp(argv[i], "--quantum=", 10) == 0) {
            const char *value = argv[i] + 10;
            config->quantum = atoi(value);
        } else if (strncmp(argv[i], "--mlfq=", 7) == 0) {
            const char *value = argv[i] + 7;
            strncpy(config->mlfq_config_file, value, sizeof(config->mlfq_config_file) - 1);
            config->mlfq_config_file[sizeof(config->mlfq_config_file) - 1] = '\0';
        } else if (strcmp(argv[i], "--compare") == 0) {
            config->compare_mode = 1;

            if (i + 1 < argc) {
                strncpy(config->input_file, argv[i + 1], sizeof(config->input_file) - 1);
                config->input_file[sizeof(config->input_file) - 1] = '\0';
                i++;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    return 1;
}

static int validate_config(const SchedulerConfig *config) {
    if (config->input_file[0] == '\0') {
        fprintf(stderr, "Error: input file is required.\n");
        return 0;
    }

    if (config->compare_mode) {
        return 1;
    }

    if (config->algorithm == ALG_NONE) {
        fprintf(stderr, "Error: valid algorithm is required.\n");
        return 0;
    }

    if (config->algorithm == ALG_RR && config->quantum <= 0) {
        fprintf(stderr, "Error: RR requires a positive --quantum value.\n");
        return 0;
    }

    if (config->algorithm == ALG_MLFQ && config->mlfq_config_file[0] == '\0') {
        fprintf(stderr, "Error: MLFQ requires --mlfq=<config_file>.\n");
        return 0;
    }

    return 1;
}

static void print_config(const SchedulerConfig *config) {
    printf("Scheduler Configuration\n");
    printf("-----------------------\n");

    if (config->compare_mode) {
        printf("Mode      : COMPARE\n");
    } else {
        printf("Algorithm : %s\n", algorithm_to_string(config->algorithm));
    }

    printf("Input     : %s\n", config->input_file);

    if (config->algorithm == ALG_RR) {
        printf("Quantum   : %d\n", config->quantum);
    }

    if (config->algorithm == ALG_MLFQ) {
        printf("MLFQ File : %s\n", config->mlfq_config_file);
    }

    printf("\n");
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
    state.process_count = load_processes(config.input_file, state.processes);

    if (state.process_count < 0) {
        return 1;
    }

    print_config(&config);
    print_processes(state.processes, state.process_count);

    if (config.algorithm == ALG_FCFS) {
        schedule_fcfs(&state);
    } else if (config.algorithm == ALG_SJF) {
        schedule_sjf(&state);
    } else {
        printf("\nSelected algorithm is not implemented yet.\n");
    }

    return 0;
}