/* All code in this source is my own. Primary source for code reference was Microsoft.learn.
* This code was based off of Dr. Qin's code. It is not the same but I did start from there.
* I did not copy/paste code directly from Phind AI but I did have it educate me and assist in solving my issues.
* COMP7500: Project 3: Aubatch
* Tucker Watts
* Auburn University
*
* This source code shows how to evaluate metrics related to job scheduling methods.
* 
* Compile with makefile using:
* make
*
* Can be run using "./aubatch" and referring to the help command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "job.h"

void *dispatcher_thread(void *arg);

void *dispatcher_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&job_queue_lock);

        while (job_count == 0) {
            pthread_cond_wait(&job_available, &job_queue_lock);
        }

        // Dequeue the first job
        last_running_job = job_queue[0];  // Store running job
        has_running_job = 1;  // Mark a job as running

        for (int i = 0; i < job_count - 1; i++) {
            job_queue[i] = job_queue[i + 1];
        }
        job_count--;

        pthread_mutex_unlock(&job_queue_lock);

        printf("\nDispatching job: %s (Execution Time: %.2f, Priority: %d)\n", last_running_job.name, last_running_job.execution_time, last_running_job.priority);

        pid_t pid = fork();  // Fork a child process
        if (pid == 0) {  // Child process
            char exe_time_str[10];
            sprintf(exe_time_str, "%.2f", last_running_job.execution_time);
            char *args[] = {"./batch_job", exe_time_str, NULL};
            execv(args[0], args);

            // If execv() fails
            perror("execv failed");
            exit(1);
        } else if (pid > 0) {  // Parent process
            wait(NULL);  // Wait for the child process to finish
            printf("\nJob %s completed.\n", last_running_job.name);

            // 🚀 Store the completed job in the completed_jobs[] list
            pthread_mutex_lock(&job_queue_lock);
            if (completed_count < MAX_COMPLETED) {
                completed_jobs[completed_count] = last_running_job;
                completed_count++;
            }
            job_index++;  // Track total jobs executed
            has_running_job = 0;  // No active job after completion
            pthread_mutex_unlock(&job_queue_lock);
        } else {
            perror("fork failed");
        }
    }
    return NULL;
}

// Function to execute a job
void execute_job(Job job) {
    printf("\nExecuting job %s for %.2f seconds (Priority: %d)\n", job.name, job.execution_time, job.priority);
    fflush(stdout); // Ensure the message prints immediately
    sleep((int)job.execution_time);
    printf("\nJob %s completed.\n", job.name);
    fflush(stdout);
}
