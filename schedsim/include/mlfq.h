#ifndef MLFQ_H
#define MLFQ_H

#define MAX_QUEUES 10

typedef struct {
    int num_queues;
    int quantum[MAX_QUEUES];
    int boost_time;   /* optional */
} MLFQConfig;

/* Parse config file */
int load_mlfq_config(const char *filename, MLFQConfig *config);

#endif