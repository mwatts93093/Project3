# AUbatch - A Pthread-based Batch Scheduling System

## Overview
AUbatch is a multi-threaded batch scheduling system implemented in C using the Pthread library. It manages batch job scheduling and execution while supporting three different scheduling policies:

1. **FCFS (First Come, First Served)** - Jobs are executed in the order they arrive.
2. **SJF (Shortest Job First)** - Jobs with shorter execution times are given priority.
3. **Priority Scheduling** - Jobs with higher priority values are executed first.

The system consists of two primary threads:
- **Scheduler Thread:** Accepts and queues submitted jobs.
- **Dispatcher Thread:** Executes jobs based on the chosen scheduling policy using `execv()`.

Synchronization between threads is achieved using **mutex locks** and **condition variables** to ensure correct handling of the job queue.

---
## Source Files
- **aubatch.c** - Main implementation file that includes:
  - Job struct and queue handling
  - Scheduler and dispatcher thread functions
  - Mutex and condition variable synchronization
  - Scheduling policy logic (FCFS, SJF, Priority Scheduling)
- **Makefile** - Build automation script to compile the program.

---
## Compilation
To compile AUbatch, run:
```bash
make
```
This will generate an executable named `aubatch`.

---
## Usage
### **Running the Program**
Execute AUbatch with:
```bash
./aubatch
```
Once started, the system automatically submits predefined jobs for execution. The scheduling policy can be changed dynamically.

### **Commands**
AUbatch supports interactive commands:
```bash
help            # Displays available commands
run <job> <time> <pri>  # Submit a job with execution time and priority
list            # Show queued jobs
fcfs            # Switch to First Come, First Served scheduling
sjf             # Switch to Shortest Job First scheduling
priority        # Switch to Priority-based scheduling
quit            # Exit the system and display performance statistics
```

### **Example Execution**
```bash
$ ./aubatch
Welcome to AUbatch - Batch Job Scheduler
Type 'help' to see available commands.

> run job1 5 2
Job job1 submitted (Exec Time: 5, Priority: 2)
> run job2 3 1
Job job2 submitted (Exec Time: 3, Priority: 1)
> list
Total Jobs: 2
Scheduling Policy: FCFS
Job1 - Exec Time: 5s, Priority: 2
Job2 - Exec Time: 3s, Priority: 1
> sjf
Scheduling policy switched to SJF. Jobs reordered.
> list
Total Jobs: 2
Scheduling Policy: SJF
Job2 - Exec Time: 3s, Priority: 1
Job1 - Exec Time: 5s, Priority: 2
> quit
Performance Summary:
- Total Jobs Executed: 2
- Average Turnaround Time: 4s
- Throughput: 0.5 jobs/sec
```

---
## Notes
- **Synchronization**: Mutex locks prevent race conditions when modifying the job queue.
- **Dynamic Scheduling**: The scheduling policy can be changed at runtime.
- **Job Execution**: Uses `execv()` to run jobs in batch mode.