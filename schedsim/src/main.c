#include <stdio.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/checker.h"
#include "../include/parser.h"

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
        printf("\nSelected algorithm is not implemented yet.\n");
    }

    validate_schedule(state.processes, state.process_count);

    return 0;
}