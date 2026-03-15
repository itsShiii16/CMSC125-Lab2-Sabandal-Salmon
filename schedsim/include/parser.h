#ifndef PARSER_H
#define PARSER_H

#include "scheduler.h"

void print_usage(const char *program_name);
int parse_args(int argc, char *argv[], SchedulerConfig *config);
int validate_config(const SchedulerConfig *config);
void print_config(const SchedulerConfig *config);

#endif
