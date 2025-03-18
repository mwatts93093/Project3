# **AUbatch - Batch Job Scheduler**

## **Overview**
AUbatch is a **multi-threaded batch job scheduling system** built in **C**, using **Pthreads** for concurrency. It supports **real-time job scheduling** and execution through a **command-line interface**, allowing users to submit, list, and manage jobs dynamically.

The system consists of **two main threads**:
- **Scheduler Thread**: Handles user input and manages the job queue.
- **Dispatcher Thread**: Executes jobs using `execv()` and tracks their execution.

It supports **three scheduling policies**:
1. **FCFS (First Come, First Served)** - Jobs are executed in the order they arrive.
2. **SJF (Shortest Job First)** - Jobs with shorter execution times are prioritized.
3. **Priority Scheduling** - Jobs with higher priority values execute first.

### **Job Execution Process**
When a job is submitted, it is stored in a **priority queue**. The dispatcher then **removes the highest priority job from the queue** and executes it as a **new process** using `execv()`, which runs an external job executable (`batch_job`).

---

## **📂 Source Files**
- **`aubatch.c`** - Interactive shell for user commands.
- **`scheduler.c`** - Manages job queuing, sorting, and scheduling policies.
- **`dispatcher.c`** - Handles job execution via `execv()`.
- **`batch_job.c`** - Simulated batch job that runs for a specified duration.
- **`job.h`** - Defines job structures, global variables, and function prototypes.
- **`Makefile`** - Automates compilation.

---

## **🛠️ Installation & Compilation**
To compile AUbatch, simply run:
```sh
make
```
This will generate the `aubatch` executable and `batch_job` program.

---

## **🚀 Running AUbatch**
Execute AUbatch with:
```sh
./aubatch
```
This starts the **interactive command-line interface** where you can submit, list, and manage jobs.

---

## **📜 Supported Commands**
| **Command** | **Description** |
|------------|---------------|
| `help` | Displays available commands. |
| `run <job> <time> <pri>` | Submits a job with execution time and priority. |
| `list` | Displays currently **queued, running, and completed jobs**. |
| `fcfs` | Switches scheduling policy to **First Come, First Served**. |
| `sjf` | Switches scheduling policy to **Shortest Job First**. |
| `priority` | Switches scheduling policy to **Priority Scheduling**. |
| `test <benchmark> <policy> <num_of_jobs> <arrival_rate> <priority_levels> <min_CPU_time> <max_CPU_time>` | Runs a performance test with simulated jobs. |
| `quit` | Exits the system and displays **performance statistics**. |

---

## **📌 Batch Job Execution (`batch_job`)**
AUbatch uses an external executable, **`batch_job`**, to simulate batch job execution. 

### **How It Works**
When a job is dispatched, it executes **`batch_job`** as a separate process using `execv()`. The `batch_job` program simply **sleeps for the specified execution time** to simulate real job processing.

### **Source Code (`batch_job.c`)**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <exe_time>\n", argv[0]);
        return 1;
    }

    int exe_time = atoi(argv[1]);
    if (exe_time <= 0) {
        fprintf(stderr, "Error: Execution time must be a positive integer.\n");
        return 1;
    }

    printf("Batch job started. Running for %d seconds...\n", exe_time);
    sleep(exe_time);
    printf("Batch job completed.\n");
    return 0;
}
```

### **Expected Behavior**
When `run job1 10 1` is issued in AUbatch, it will:
1. **Fork a new process**.
2. **Execute `batch_job` with the argument `10`**.
3. **The batch job runs for 10 seconds**.
4. **After completion, the dispatcher marks the job as completed**.

```sh
> run job1 10 1
Dispatching job: job1 (Execution Time: 10, Priority: 1)
Batch job started. Running for 10 seconds...
Batch job completed.
Job job1 completed.
```

---

## **📌 Example Usage**
```sh
$ ./aubatch
Welcome to AUbatch - Batch Job Scheduler
Type 'help' to see available commands.

> run job1 10 1
Job job1 submitted (Exec Time: 10, Priority: 1)

> run job2 5 2
Job job2 submitted (Exec Time: 5, Priority: 2)

> list
=====================================
         JOB QUEUE STATUS           
=====================================
Total Jobs in Queue: 2
Total Jobs Executed: 0
Scheduling Policy: FCFS

Name       CPU_Time   Pri   Status      
---------------------------------
job1       10        1     Active      
job2       5         2     Queued      

=====================================
         COMPLETED JOBS              
=====================================
No jobs have been completed yet.
```