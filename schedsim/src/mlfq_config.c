#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "../include/mlfq.h"

static int parse_nonnegative_int(const char *value, int *out)
{
    char *endptr;
    long parsed;

    errno = 0;
    parsed = strtol(value, &endptr, 10);

    if (value == endptr) {
        return 0;
    }

    if (*endptr != '\0' && *endptr != '\n') {
        return 0;
    }

    if (errno == ERANGE || parsed > INT_MAX || parsed < INT_MIN) {
        return 0;
    }

    if (parsed < 0) {
        return 0;
    }

    *out = (int)parsed;
    return 1;
}

static int parse_positive_int(const char *value, int *out)
{
    if (!parse_nonnegative_int(value, out)) {
        return 0;
    }

    return *out > 0;
}

int load_mlfq_config(const char *filename, MLFQConfig *config)
{
    FILE *file = fopen(filename, "r");

    if (!file) {
        perror("Error opening MLFQ config file");
        return 0;
    }

    char line[256];

    config->num_queues = 0;
    config->boost_time = -1;

    for (int i = 0; i < MAX_QUEUES; i++) {
        config->quantum[i] = 0;
    }

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "queues=", 7) == 0) {
            if (!parse_positive_int(line + 7, &config->num_queues)) {
                fprintf(stderr, "Error: invalid queues value in MLFQ config.\n");
                fclose(file);
                return 0;
            }

            if (config->num_queues > MAX_QUEUES) {
                fprintf(stderr, "Error: queues value exceeds MAX_QUEUES (%d).\n", MAX_QUEUES);
                fclose(file);
                return 0;
            }
        }

        else if (strncmp(line, "boost=", 6) == 0) {
            if (!parse_nonnegative_int(line + 6, &config->boost_time)) {
                fprintf(stderr, "Error: invalid boost value in MLFQ config.\n");
                fclose(file);
                return 0;
            }
        }

        else if (line[0] == 'q') {
            int level;
            int quantum;
            char *equals = strchr(line, '=');

            if (!equals) {
                fprintf(stderr, "Error: invalid queue quantum format in MLFQ config.\n");
                fclose(file);
                return 0;
            }

            *equals = '\0';

            if (sscanf(line, "q%d", &level) != 1) {
                fprintf(stderr, "Error: invalid queue level in MLFQ config.\n");
                fclose(file);
                return 0;
            }

            if (level < 0 || level >= MAX_QUEUES) {
                fprintf(stderr, "Error: queue level q%d is out of range.\n", level);
                fclose(file);
                return 0;
            }

            if (!parse_positive_int(equals + 1, &quantum)) {
                fprintf(stderr, "Error: invalid quantum for q%d in MLFQ config.\n", level);
                fclose(file);
                return 0;
            }

            config->quantum[level] = quantum;
        }
    }

    fclose(file);

    if (config->num_queues <= 0) {
        fprintf(stderr, "Error: invalid MLFQ config, queues not defined.\n");
        return 0;
    }

    for (int i = 0; i < config->num_queues; i++) {
        if (config->quantum[i] <= 0) {
            fprintf(stderr, "Error: missing or invalid quantum for q%d in MLFQ config.\n", i);
            return 0;
        }
    }

    return 1;
}