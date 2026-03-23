# CPU Scheduling Simulator

### CMSC 125 – Operating Systems

**University of the Philippines Visayas**

---

## Overview

Modern operating systems rely on CPU scheduling algorithms to determine which process executes at any given time. Different strategies optimize fairness, responsiveness, and throughput.

This project is a **CPU Scheduling Simulator written in C** that allows users to experiment with multiple scheduling algorithms using either **input files** or **inline command-line input**. It computes performance metrics and generates **ASCII Gantt charts** for visualization.

---

## Features

* Multi-Algorithm Simulation (FCFS, SJF, STCF, RR, MLFQ)
* File-based and Inline Input Support
* Discrete-Time Simulation Engine
* ASCII Gantt Chart Visualization
* Performance Metrics (TT, WT, RT)
* MLFQ Configurable via File
* Validation Checker
* Modular and Extensible Code Structure

---

## Supported Scheduling Algorithms

| Algorithm | Type           | Description                       |
| --------- | -------------- | --------------------------------- |
| **FCFS**  | Non-preemptive | First-Come First-Serve            |
| **SJF**   | Non-preemptive | Shortest Job First                |
| **STCF**  | Preemptive     | Shortest Time to Completion First |
| **RR**    | Preemptive     | Round Robin                       |
| **MLFQ**  | Adaptive       | Multi-Level Feedback Queue        |

---

## Project Structure

```
schedsim/
├── include/
│   ├── process.h
│   ├── scheduler.h
│   ├── metrics.h
│   ├── utils.h
│   ├── gantt.h
│   ├── mlfq.h
│
├── src/
│   ├── main.c
│   ├── process.c
│   ├── parser.c
│   ├── checker.c
│   ├── metrics.c
│   ├── utils.c
│   ├── gantt.c
│   ├── fcfs.c
│   ├── sjf.c
│   ├── stcf.c
│   ├── rr.c
│   ├── mlfq.c
│   ├── mlfq_config.c
│
├── tests/
│   ├── workload1.txt
│   ├── workload2.txt
│   ├── mlfq.txt
│
├── docs/
│   └── design.md
│
├── Makefile
└── README.md
```

---

## System Design Hierarchy

### High-Level Flow

```
User Input (CLI / File / Inline)
        ↓
Argument Parsing (main.c)
        ↓
Workload Loading
  ├── File Input (process.c)
  └── Inline Input (parser.c)
        ↓
Scheduler Core (scheduler.h)
        ↓
Selected Algorithm
  ├── FCFS
  ├── SJF
  ├── STCF
  ├── RR
  └── MLFQ
        ↓
Execution Engine (time simulation)
        ↓
Metrics Computation (metrics.c)
        ↓
Validation Checker (checker.c)
        ↓
Gantt Chart Generator (gantt.c)
        ↓
Console Output (results + visualization)
```

---

## Installation & Compilation

### Requirements

* GCC
* Make

### Build

```
make
```

### Clean

```
make clean
```

---

## How to Run the Scheduler

The simulator supports **two modes of input**:

---

### 1. Using Workload Files

#### Format

```
PID ArrivalTime BurstTime
```

#### Example (`tests/workload1.txt`)

```
A 0 5
B 1 3
C 2 8
```

#### Run Commands

**FCFS**

```
./schedsim --algorithm=FCFS --input=tests/workload1.txt
```

**SJF**

```
./schedsim --algorithm=SJF --input=tests/workload1.txt
```

**STCF**

```
./schedsim --algorithm=STCF --input=tests/workload1.txt
```

**Round Robin**

```
./schedsim --algorithm=RR --quantum=2 --input=tests/workload1.txt
```

**MLFQ**

```
./schedsim --algorithm=MLFQ --mlfq=tests/mlfq.txt --input=tests/workload1.txt
```

---

### 2. Using Inline Input (Direct Parsing)

#### Format

```
"PID Arrival Burst; PID Arrival Burst; ..."
```

#### Example

```
./schedsim --algorithm=SJF --processes="A 0 5; B 1 3; C 2 8"
```

#### More Examples

**Round Robin**

```
./schedsim --algorithm=RR --quantum=2 --processes="A 0 5; B 1 3; C 2 8"
```

**MLFQ**

```
./schedsim --algorithm=MLFQ --mlfq=tests/mlfq.txt --processes="A 0 5; B 1 3; C 2 8"
```

---

### 3. Compare Mode

Run all algorithms on one workload:

```
./schedsim --compare tests/workload1.txt
```

---

## MLFQ Configuration File

Example (`tests/mlfq.txt`):

```
queues=3
q0=2
q1=4
q2=8
boost=50
```

---

## Example Output

### Gantt Chart

```
| A | B | C |
0    5    8    10
```

### Metrics Table

```
PID AT BT ST FT TT WT RT
A   0  5  0  5  5  0  0
B   1  3  5  8  7  4  4
C   2  8  8 16 14  6  6
```

---

## Metrics Explained

* **Turnaround Time (TT)** = Finish Time - Arrival Time
* **Waiting Time (WT)** = TT - Burst Time
* **Response Time (RT)** = First Start Time - Arrival Time

---

## Limitations

* Single CPU simulation only
* No I/O blocking or multi-core support
* ASCII-only visualization

---

## Authors

* Sherwin Paul Sabandal
* Antonio Gabriel Salmon

University of the Philippines Visayas

---

## License

This project is developed for academic purposes under CMSC 125 – Operating Systems.
