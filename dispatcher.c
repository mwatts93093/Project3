#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "job.h"

void *dispatcher_thread(void *arg);
void execute_job(Job job);

void *dispatcher_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&job_queue_lock);

        while (job_count == 0) {
            pthread_cond_wait(&job_available, &job_queue_lock);
        }

        // Dequeue the first job
        last_running_job = job_queue[0];
        has_running_job = 1;

        for (int i = 0; i < job_count - 1; i++) {
            job_queue[i] = job_queue[i + 1];
        }
        job_count--;

        pthread_mutex_unlock(&job_queue_lock);

        printf("\nExecuting job %s for %d seconds (Priority: %d)\n", last_running_job.name, last_running_job.execution_time, last_running_job.priority);
        fflush(stdout);
        sleep(last_running_job.execution_time);
        printf("\nJob %s completed.\n", last_running_job.name);
        fflush(stdout);

        // Store job in completed_jobs
        pthread_mutex_lock(&job_queue_lock);
        if (completed_count < MAX_COMPLETED) {
            completed_jobs[completed_count] = last_running_job;
            completed_count++;
        }
        has_running_job = 0;
        job_index++;
        pthread_mutex_unlock(&job_queue_lock);
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
