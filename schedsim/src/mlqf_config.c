#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/mlfq.h"

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

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "queues=", 7) == 0) {
            config->num_queues = atoi(line + 7);
        }

        else if (strncmp(line, "boost=", 6) == 0) {
            config->boost_time = atoi(line + 6);
        }

        else if (line[0] == 'q') {
            int level, quantum;

            if (sscanf(line, "q%d=%d", &level, &quantum) == 2) {
                if (level >= 0 && level < MAX_QUEUES) {
                    config->quantum[level] = quantum;
                }
            }
        }
    }

    fclose(file);

    if (config->num_queues <= 0) {
        printf("Invalid MLFQ config: queues not defined\n");
        return 0;
    }

    return 1;
}