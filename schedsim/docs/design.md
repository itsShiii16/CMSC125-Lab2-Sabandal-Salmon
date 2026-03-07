# CPU Scheduling Simulator Design Plan
CMSC 125 – Operating Systems  
Laboratory Activity: CPU Scheduling Simulator

This document outlines the design strategy, implementation roadmap, and development phases for the CPU scheduling simulator. The goal of this project is to build a modular simulator written in C that implements several CPU scheduling algorithms and allows comparative analysis of their performance.

---

# 1. Project Overview

The simulator will execute workloads consisting of processes with arrival times and CPU burst times. The system will simulate CPU scheduling policies and compute performance metrics such as turnaround time, waiting time, and response time.

The simulator will support the following scheduling algorithms:

- First-Come First-Serve (FCFS)
- Shortest Job First (SJF)
- Shortest Time to Completion First (STCF)
- Round Robin (RR)
- Multi-Level Feedback Queue (MLFQ)

The simulator will also generate ASCII Gantt charts to visualize execution timelines and provide comparison outputs across algorithms.

---

# 2. Project Architecture

The system is designed using modular components to separate concerns and improve maintainability.

## 2.1 Core Components

The simulator is divided into the following modules:

### Process Management
Responsible for representing processes and loading workloads from input files.

Responsibilities:
- Process data structure
- Workload file parsing
- Process initialization
- Tracking runtime state

### Scheduling Engine
Controls the simulation of time and determines which process runs at each time unit.

Responsibilities:
- Simulation loop
- Process arrival handling
- Ready queue management
- Algorithm selection

### Scheduling Algorithms
Each scheduling algorithm is implemented as a separate module.

Algorithms:
- FCFS (non-preemptive)
- SJF (non-preemptive)
- STCF (preemptive)
- RR (preemptive)
- MLFQ (adaptive priority scheduler)

### Metrics Module
Calculates performance statistics for each process and averages across workloads.

Metrics:
- Finish Time
- Turnaround Time
- Waiting Time
- Response Time

### Visualization Module
Responsible for generating ASCII Gantt charts showing process execution timelines.

### CLI Interface
Handles command-line arguments and allows users to select scheduling algorithms and configuration parameters.

---

# 3. Repository Structure


The project will follow a modular directory layout.
schedsim/
├── Makefile
├── README.md
│
├── include/
│ ├── process.h
│ ├── scheduler.h
│ ├── metrics.h
│ └── gantt.h
│
├── src/
│ ├── main.c
│ ├── process.c
│ ├── fcfs.c
│ ├── sjf.c
│ ├── stcf.c
│ ├── rr.c
│ ├── mlfq.c
│ ├── metrics.c
│ ├── gantt.c
│ └── utils.c
│
├── tests/
│ ├── workload1.txt
│ ├── workload2.txt
│ └── test_suite.sh
│
└── docs/
└── design.md


---

# 4. Development Phases

The laboratory activity follows a four-week development timeline consisting of progress reports and a final defense.

---

# Phase 1 — System Design and Setup
Week 1

Objective: Establish the architectural foundation of the simulator.

Tasks:
- Analyze scheduling requirements
- Design system architecture
- Create repository structure
- Define process data structures
- Define scheduler state structures
- Implement workload parser
- Set up Makefile build system
- Create initial documentation

Deliverables:
- Project folder structure
- Initial commits in repository
- Design documentation (`design.md`)
- Process data structure definition

Expected Output:
A compilable program skeleton capable of loading workloads.

---

# Phase 2 — Functional Prototype
Week 2

Objective: Implement core scheduling algorithms and the simulation engine.

Tasks:
- Implement simulation time loop
- Implement ready queue logic
- Implement FCFS scheduler
- Implement SJF scheduler
- Implement STCF scheduler
- Implement Round Robin scheduler
- Implement command-line argument parsing

Deliverables:
- Functional prototype
- Working implementations of basic algorithms
- Documented source code
- Demonstration of scheduler outputs

Expected Output:
Simulator capable of running scheduling algorithms and displaying execution traces.

---

# Phase 3 — Complete Implementation
Week 3

Objective: Extend the simulator with metrics, visualization, and advanced scheduling.

Tasks:
- Implement scheduling metrics calculations
- Implement ASCII Gantt chart generation
- Implement comparison mode across algorithms
- Implement Multi-Level Feedback Queue (MLFQ)
- Implement priority boost mechanism
- Implement queue demotion logic
- Implement MLFQ allotment tracking

Deliverables:
- Full algorithm implementations
- Metrics output
- Visualization of scheduling results
- Test workloads and validation

Expected Output:
Fully functional simulator capable of analyzing scheduling performance.

---

# Phase 4 — Testing and Defense Preparation
Week 4

Objective: Validate correctness and prepare for laboratory defense.

Tasks:
- Implement automated test suite
- Test edge cases
- Verify correctness of scheduling algorithms
- Perform performance analysis on workloads
- Generate demonstration outputs
- Prepare screenshots and logs
- Finalize documentation

Deliverables:
- Automated test scripts
- Test workload files
- Output logs
- Repository ready for submission
- Defense-ready implementation

Expected Output:
Stable simulator implementation ready for demonstration and evaluation.

---

# 5. Testing Strategy

Testing will include both correctness verification and edge-case analysis.

## Test Cases

The following scenarios will be tested:

- Single process workloads
- Simultaneous process arrivals
- Identical burst times
- Zero burst-time processes
- Long workloads with many processes
- Mixed workloads (short and long jobs)

The automated test suite will run each scheduling algorithm and verify expected metrics.

---

# 6. Future Improvements

Potential improvements beyond the lab requirements include:

- Graphical visualization of Gantt charts
- Interactive workload generation
- Additional scheduling algorithms
- Performance benchmarking tools

---

# 7. Summary

This project implements a modular CPU scheduling simulator that demonstrates the behavior and trade-offs of different scheduling algorithms. The phased development plan ensures that the system is built incrementally, allowing for testing, iteration, and refinement throughout the development process.


