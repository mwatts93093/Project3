/* All code in this source is my own. Primary source for code reference was Microsoft.learn.
* This code was based off of Dr. Qin's code. It is not the same but I did start from there.
* I did not copy/paste code directly from Phind AI but I did have it educate me and assist in solving my issues.
* COMP7500: Project 3: Aubatch
* Tucker Watts
* Auburn University
*
* TThis source code shows how to evaluate metrics related to job scheduling methods.
* 
* Compile with makefile using:
* make
*
* Can be run using "./aubatch" and referring to the help command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "job.h"

void shell_loop();
int parse_scheduling_policy(char *policy_str);

int main() {
    pthread_mutex_init(&job_queue_lock, NULL);
    pthread_cond_init(&job_available, NULL);

    pthread_t dispatcher;
    pthread_create(&dispatcher, NULL, dispatcher_thread, NULL);

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
        // Print prompt only when a command is expected
        printf("> ");
        fflush(stdout);

        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline

        // If the user just presses Enter, refresh the prompt without printing another ">"
        if (strlen(command) == 0) {
            continue;  // Go back to the top of the loop, reprinting the prompt just once
        }


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
                submit_job(job_name, time, pri, 1);  // show_output = 1 (Print Messages)
            } else {
                printf("Invalid usage. Example: run job1 5 2\n");
            }
        } else if (strncmp(command, "test", 4) == 0) {
            char benchmark[50], policy_str[10];
            int policy, num_of_jobs, priority_levels;
            double arrival_rate, min_CPU_time, max_CPU_time;
        
            if (sscanf(command, "test %s %s %d %lf %d %lf %lf", benchmark, policy_str, &num_of_jobs, &arrival_rate, &priority_levels, &min_CPU_time, &max_CPU_time) == 7) {
                policy = parse_scheduling_policy(policy_str);
                
                if (policy != -1) {
                    Job jobs[num_of_jobs];
        
                    // Generate test jobs
                    for (int i = 0; i < num_of_jobs; i++) {
                        jobs[i].execution_time = min_CPU_time + (rand() % (int)(max_CPU_time - min_CPU_time + 1));
                        jobs[i].priority = rand() % priority_levels + 1;
                    }
        
                    // Call evaluation function
                    evaluate_performance(jobs, num_of_jobs, policy);
                } else {
                    printf("Invalid policy. Use fcfs, sjf, or priority.\n");
                }
            } else {
                printf("Invalid usage. Example: test mybenchmark fcfs 5 0.5 3 10 20\n");
            }
        }
        else if (strncmp(command, "list", 4) == 0) {
            pthread_mutex_lock(&job_queue_lock);
        
            printf("=====================================\n");
            printf("         JOB QUEUE STATUS           \n");
            printf("=====================================\n");
        
            printf("Total number of jobs in the queue: %d\n", job_count);
            printf("Total number of jobs executed: %d\n", job_index);
            printf("Scheduling Policy: %s\n", scheduling_policy == 0 ? "FCFS" : scheduling_policy == 1 ? "SJF" : "Priority");
        
            if (job_count == 0 && !has_running_job) {
                printf("No jobs currently in queue.\n");
            } else {
                printf("\n%-10s %-10s %-5s %-12s\n", "Name", "CPU_Time", "Pri", "Status");
                printf("-------------------------------------------------\n");
        
                // 🚀 Show currently running job
                if (has_running_job) {
                    printf("%-10s %-10.2f %-5d %-12s\n",
                           last_running_job.name,
                           last_running_job.execution_time,
                           last_running_job.priority,
                           "Running");
                }
        
                // Show queued jobs
                for (int i = 0; i < job_count; i++) {
                    printf("%-10s %-10d %-5d %-12s\n",
                           job_queue[i].name,
                           job_queue[i].execution_time,
                           job_queue[i].priority,
                           "Queued");
                }
            }
        
            // 🚀 Completed Jobs Section
            printf("\n=====================================\n");
            printf("         COMPLETED JOBS              \n");
            printf("=====================================\n");
        
            if (completed_count == 0) {
                printf("No jobs have been completed yet.\n");
            } else {
                printf("\n%-10s %-10s %-5s %-12s\n", "Name", "CPU_Time", "Pri", "Completion Time");
                printf("-------------------------------------------------\n");
        
                for (int i = 0; i < completed_count; i++) {
                    printf("%-10s %-10.2f %-5d %-12s\n",
                           completed_jobs[i].name,
                           completed_jobs[i].execution_time,
                           completed_jobs[i].priority,
                           "Completed");
                }
            }
        
            pthread_mutex_unlock(&job_queue_lock);
        }
         else if (strncmp(command, "fcfs", 4) == 0) {
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
            printf("=================================\n");
            printf("       PERFORMANCE SUMMARY       \n");
            printf("=================================\n");
            printf("Total Jobs Executed: %d\n", job_index);
        
            if (job_index > 0) {
                double avg_turnaround_time = 0.0, avg_cpu_time = 0.0, avg_waiting_time = 0.0;
        
                for (int i = 0; i < completed_count; i++) {
                    avg_turnaround_time += response_times[i] + completed_jobs[i].execution_time;
                    avg_cpu_time += completed_jobs[i].execution_time;
                }
        
                avg_turnaround_time /= completed_count;
                avg_cpu_time /= completed_count;
                avg_waiting_time = avg_turnaround_time - avg_cpu_time;
        
                printf("Average Turnaround Time: %.2f seconds\n", avg_turnaround_time);
                printf("Average CPU Time: %.2f seconds\n", avg_cpu_time);
                printf("Average Waiting Time: %.2f seconds\n", avg_waiting_time);
            } else {
                printf("No jobs were executed.\n");
            }
        
            exit(0);
        }
        
        else {
            printf("Unknown command. Type 'help' for available commands.\n");
        }
    }
}