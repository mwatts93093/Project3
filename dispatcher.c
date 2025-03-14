#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "job.h"

void *dispatcher_thread(void *arg);
void execute_job(Job job);

// Dispatcher Thread: Continuously executes jobs
void *dispatcher_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&job_queue_lock);

        // Wait until a job is available
        while (job_count == 0) {
            pthread_cond_wait(&job_available, &job_queue_lock);
        }

        // Dequeue the first job
        Job job = job_queue[0];

        // Shift remaining jobs forward
        for (int i = 0; i < job_count - 1; i++) {
            job_queue[i] = job_queue[i + 1];
        }
        job_count--;

        pthread_mutex_unlock(&job_queue_lock);

        // Execute the job
        execute_job(job);
        
        // Track jobs executed
        pthread_mutex_lock(&job_queue_lock);
        job_index++;  // Increment job count for statistics tracking
        pthread_mutex_unlock(&job_queue_lock);
    }
    return NULL;
}

// Function to execute a job
void execute_job(Job job) {
    printf("Executing job %s for %d seconds (Priority: %d)\n", job.name, job.execution_time, job.priority);
    sleep(job.execution_time);
    printf("Job %s completed.\n", job.name);
}
