#ifndef METRICS_H
#define METRICS_H

#include "process.h"

void compute_metrics(Process processes[], int count);
void print_results(Process processes[], int count);
void print_averages(Process processes[], int count);

#endif