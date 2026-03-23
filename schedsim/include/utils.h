#ifndef UTILS_H
#define UTILS_H

#include "process.h"

/* Reset all process fields before scheduling */
void reset_processes(Process processes[], int count);

/* Check if all processes are completed */
int all_processes_completed(Process processes[], int count);

#endif