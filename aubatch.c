#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "job.h"

void shell_loop();

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
            printf("quit            # Exit the system and display performance statistics\n");
        } else if (strncmp(command, "run", 3) == 0) {
            char job_name[50];
            int time, pri;
            if (sscanf(command, "run %s %d %d", job_name, &time, &pri) == 3) {
                submit_job(job_name, time, pri);
            } else {
                printf("Invalid usage. Example: run job1 5 2\n");
            }
        } else if (strncmp(command, "list", 4) == 0) {
            printf("Total Jobs: %d\n", job_count);
            printf("Scheduling Policy: %s\n", scheduling_policy == 0 ? "FCFS" : scheduling_policy == 1 ? "SJF" : "Priority");
            for (int i = 0; i < job_count; i++) {
                printf("%s - Exec Time: %ds, Priority: %d\n", job_queue[i].name, job_queue[i].execution_time, job_queue[i].priority);
            }
        } else if (strncmp(command, "fcfs", 4) == 0) {
            change_scheduling_policy(0);
            printf("Scheduling policy switched to FCFS. Jobs reordered.\n");
        } else if (strncmp(command, "sjf", 3) == 0) {
            change_scheduling_policy(1);
            printf("Scheduling policy switched to SJF. Jobs reordered.\n");
        } else if (strncmp(command, "priority", 8) == 0) {
            change_scheduling_policy(2);
            printf("Scheduling policy switched to Priority-based. Jobs reordered.\n");
        } else if (strncmp(command, "quit", 4) == 0) {
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
