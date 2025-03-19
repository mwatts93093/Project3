# **AUbatch - Batch Job Scheduler**

## **Overview**
AUbatch is a **multi-threaded batch job scheduling system** built in **C**, utilizing **Pthreads** for concurrency. It supports **real-time job scheduling** and execution via a **command-line interface**, allowing users to submit, manage, and track jobs dynamically.

The system consists of **two main threads**:
- **Scheduler Thread**: Handles user input and manages the job queue.
- **Dispatcher Thread**: Executes jobs using `execv()` and tracks their execution.

It supports **three scheduling policies**:
1. **FCFS (First Come, First Served)** - Jobs execute in the order they arrive.
2. **SJF (Shortest Job First)** - Jobs with shorter execution times are prioritized.
3. **Priority Scheduling** - Jobs with higher priority values execute first.

### **Job Execution Process**
When a job is submitted, it is stored in a **queue**. The dispatcher removes the **highest priority job** and executes it as a **new process** using `execv()`, which runs an external job executable (`batch_job`).

---

## **📂 Source Files**
- **`aubatch.c`** - Interactive shell for user commands.
- **`scheduler.c`** - Manages job queuing, sorting, and scheduling policies.
- **`dispatcher.c`** - Handles job execution via `execv()`.
- **`evaluation.c`** - Evaluates job performance metrics.
- **`batch_job.c`** - Simulated batch job execution.
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
When a job is dispatched, it executes **`batch_job`** as a separate process using `execv()`. The `batch_job` program simulates job execution by **performing CPU-intensive calculations and logging progress**.

### **Source Code (`batch_job.c`)**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <exe_time>\n", argv[0]);
        return 1;
    }

    double exe_time = atof(argv[1]);
    if (exe_time <= 0) {
        fprintf(stderr, "Error: Execution time must be a positive number.\n");
        return 1;
    }

    printf("Batch job started. Running for %.2f seconds...\n", exe_time);

    FILE *log_file = fopen("batch_job.log", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return 1;
    }

    fprintf(log_file, "Batch job started. Estimated duration: %.2f seconds\n", exe_time);

    double result = 0.0;
    clock_t start_time = clock();
    while ((double)(clock() - start_time) / CLOCKS_PER_SEC < exe_time) {
        for (int i = 0; i < 1000000; i++) {
            result += sin(i) * cos(i);
        }
        fprintf(log_file, "Progress: %.2f%%\n", ((double)(clock() - start_time) / CLOCKS_PER_SEC / exe_time) * 100);
    }

    fprintf(log_file, "Batch job completed. Result: %.5f\n", result);
    fclose(log_file);

    printf("Batch job completed.\n");
    return 0;
}
```

### **Expected Behavior**
When `run job1 10 1` is issued in AUbatch, it will:
1. **Fork a new process**.
2. **Execute `batch_job` with the argument `10`**.
3. **Perform CPU-intensive calculations and log execution progress**.
4. **After completion, the dispatcher marks the job as completed**.

```sh
> run job1 10 1
Dispatching job: job1 (Execution Time: 10, Priority: 1)
Batch job started. Running for 10 seconds...
Batch job completed.
Job job1 completed.
```

---

## **📊 Performance Metrics & Testing**
AUbatch includes a **test command** that allows users to **benchmark different scheduling policies** under varying workload conditions. The test evaluates:
- **Turnaround Time** (Total time from job submission to completion)
- **CPU Time** (Actual execution duration)
- **Waiting Time** (Time spent in queue before execution)
- **Throughput** (Number of jobs completed per second)

### **Example Test Run**
```sh
> test mybench fcfs 10 0.5 5 1 10
Total Jobs Submitted: 10
Average Turnaround Time: 20.5 seconds
Average CPU Time: 5.3 seconds
Average Waiting Time: 15.2 seconds
Throughput: 0.49 No./second
```

This helps compare scheduling performance under different workloads.

---

## **📌 Example Usage**
```sh
$ ./aubatch
Welcome to AUbatch - Batch Job Scheduler
Type 'help' to see available commands.

> run job1 10 1
Job job1 submitted (Exec Time: 10, Priority: 1)

> list
=====================================
         JOB QUEUE STATUS           
=====================================
Total Jobs in Queue: 1
Total Jobs Executed: 0
Scheduling Policy: FCFS

Name       CPU_Time   Pri   Status      
---------------------------------
job1       10        1     Active      

=====================================
         COMPLETED JOBS              
=====================================
No jobs have been completed yet.
```

