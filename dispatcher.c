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
        Job job = job_queue[0];
        job.start_time = time(NULL);  // Capture start time

        for (int i = 0; i < job_count - 1; i++) {
            job_queue[i] = job_queue[i + 1];
        }
        job_count--;

        pthread_mutex_unlock(&job_queue_lock);

        // Execute the job
        execute_job(job);

        // Calculate and store response time
        pthread_mutex_lock(&job_queue_lock);
        if (completed_count < MAX_COMPLETED) {
            completed_jobs[completed_count] = job;
            response_times[completed_count] = difftime(job.start_time, job.submission_time);
            completed_count++;
        }
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
