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
        Job job = job_queue[0];

        for (int i = 0; i < job_count - 1; i++) {
            job_queue[i] = job_queue[i + 1];
        }
        job_count--;

        pthread_mutex_unlock(&job_queue_lock);

        printf("\nDispatching job: %s (Execution Time: %d, Priority: %d)\n", job.name, job.execution_time, job.priority);

        pid_t pid = fork();  // Fork a child process
        if (pid == 0) {  // Child process
            char exe_time_str[10];
            sprintf(exe_time_str, "%d", job.execution_time);
            char *args[] = {"./batch_job", exe_time_str, NULL};
            execv(args[0], args);

            // If execv() fails
            perror("execv failed");
            exit(1);
        } else if (pid > 0) {  // Parent process
            wait(NULL);  // Wait for the child process to finish
            printf("\nJob %s completed.\n", job.name);

            // 🚀 Store the completed job in the completed_jobs[] list
            pthread_mutex_lock(&job_queue_lock);
            if (completed_count < MAX_COMPLETED) {
                completed_jobs[completed_count] = job;
                completed_count++;
            }
            job_index++;  // Track total jobs executed
            pthread_mutex_unlock(&job_queue_lock);
        } else {
            perror("fork failed");
        }
    }
    return NULL;
}

// Function to execute a job
void execute_job(Job job) {
    printf("\nExecuting job %s for %d seconds (Priority: %d)\n", job.name, job.execution_time, job.priority);
    fflush(stdout); // Ensure the message prints immediately
    sleep(job.execution_time);
    printf("\nJob %s completed.\n", job.name);
    fflush(stdout);
}
