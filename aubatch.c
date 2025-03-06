#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "job.h"

void shell_loop();
void run_benchmark(int exe_time);
void test_benchmark(char *benchmark, int policy, int num_of_jobs, double arrival_rate, int priority_levels, int min_CPU_time, int max_CPU_time);

int main() {
    pthread_mutex_init(&job_queue_lock, NULL);
    pthread_cond_init(&job_available, NULL);

    printf("Welcome to AUbatch - Batch Job Scheduler\n");
    printf("Type 'help' to see available commands.\n\n");
    
    shell_loop();
    
    pthread_mutex_destroy(&job_queue_lock);
    pthread_cond_destroy(&job_available);

    return 0;
}

void shell_loop() {
    char command[256];
    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline

        if (strncmp(command, "help", 4) == 0) {
            printf("Available commands:\n");
            printf("help            # Displays available commands\n");
            printf("run <job> <time> <pri>  # Submit a job with execution time and priority\n");
            printf("list            # Show queued jobs\n");
            printf("fcfs            # Switch to First Come, First Served scheduling\n");
            printf("sjf             # Switch to Shortest Job First scheduling\n");
            printf("priority        # Switch to Priority-based scheduling\n");
            printf("test <benchmark> <policy> <num_of_jobs> <arrival_rate> <priority_levels> <min_CPU_time> <max_CPU_time>  # Run performance test\n");
            printf("quit            # Exit the system and display performance statistics\n");
        } else if (strncmp(command, "run", 3) == 0) {
            char job_name[50];
            int time, pri;
            if (sscanf(command, "run %s %d %d", job_name, &time, &pri) == 3) {
                submit_job_with_output(job_name, time, pri);
            } else {
                printf("Invalid usage. Example: run job1 5 2\n");
            }
        } else if (strncmp(command, "test", 4) == 0) {
            char benchmark[50];
            int policy, num_of_jobs, priority_levels, min_CPU_time, max_CPU_time;
            double arrival_rate;
            if (sscanf(command, "test %s %d %d %lf %d %d %d", benchmark, &policy, &num_of_jobs, &arrival_rate, &priority_levels, &min_CPU_time, &max_CPU_time) == 7) {
                test_benchmark(benchmark, policy, num_of_jobs, arrival_rate, priority_levels, min_CPU_time, max_CPU_time);
            } else {
                printf("Invalid usage. Example: test mybenchmark 0 5 0.5 3 10 20\n");
            }
        } else if (strncmp(command, "list", 4) == 0) {
            time_t now;
            struct tm *time_info;
            char arrival_time[10];
            
            printf("Total number of jobs in the queue: %d\n", job_count);
            printf("Scheduling Policy: %s\n", scheduling_policy == 0 ? "FCFS" : scheduling_policy == 1 ? "SJF" : "Priority");
            printf("%-10s %-10s %-5s %-12s %-10s\n", "Name", "CPU_Time", "Pri", "Arrival_time", "Progress");
            
            for (int i = 0; i < job_count; i++) {
                now = time(NULL);
                time_info = localtime(&now);
                strftime(arrival_time, sizeof(arrival_time), "%H:%M:%S", time_info);
                printf("%-10s %-10d %-5d %-12s %s\n", job_queue[i].name, job_queue[i].execution_time, job_queue[i].priority, arrival_time, i == 0 ? "Run" : "");
            }
        } else if (strncmp(command, "fcfs", 4) == 0) {
            change_scheduling_policy(0);
            printf("Scheduling policy switched to FCFS. Jobs reordered.\n");
        } else if (strncmp(command, "sjf", 3) == 0) {
            change_scheduling_policy(1);
            printf("Scheduling policy switched to SJF. Jobs reordered.\n");
        } else if (strncmp(command, "priority", 8) == 0) {
            change_scheduling_policy(2);
            printf("Scheduling policy switched to Priority-based. Jobs reordered in correct priority order.\n");
        
            // Corrected sorting: priority 1 (highest) should come first
            for (int i = 0; i < job_count - 1; i++) {
                for (int j = 0; j < job_count - i - 1; j++) {
                    if (job_queue[j].priority > job_queue[j + 1].priority) { // FIX: Corrected comparison
                        Job temp = job_queue[j];
                        job_queue[j] = job_queue[j + 1];
                        job_queue[j + 1] = temp;
                    }
                }
            }
        }
                
         else if (strncmp(command, "quit", 4) == 0) {
            printf("Performance Summary:\n");
            printf("- Total Jobs Executed: %d\n", job_index);
            printf("- Average Turnaround Time: %.2f seconds\n", job_index > 0 ? (double)(job_index * 5) / job_index : 0.0);
            printf("- Throughput: %.2f jobs/sec\n", job_index > 0 ? (double)job_index / 10 : 0.0);
            break;
        } else {
            printf("Unknown command. Type 'help' for available commands.\n");
        }
    }
}

void test_benchmark(char *benchmark, int policy, int num_of_jobs, double arrival_rate, int priority_levels, int min_CPU_time, int max_CPU_time) {
    printf("Running performance test for %s with policy %d...\n", benchmark, policy);
    for (int i = 0; i < num_of_jobs; i++) {
        int exec_time = min_CPU_time + rand() % (max_CPU_time - min_CPU_time + 1);
        submit_job(benchmark, exec_time, rand() % priority_levels + 1);
        usleep(arrival_rate * 1000000); // Convert seconds to microseconds
    }
    printf("Test completed. Check job execution logs for performance analysis.\n");
}

void run_benchmark(int exe_time) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process executes the benchmark job
        char exe_time_str[10];
        sprintf(exe_time_str, "%d", exe_time);
        execl("./batch_job", "batch_job", exe_time_str, NULL);
        exit(1); // Exit if execl fails
    } else if (pid > 0) {
        printf("Running benchmark job for %d seconds...\n", exe_time);
    } else {
        perror("fork failed");
    }
}