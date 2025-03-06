#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job.h"

Job job_queue[MAX_JOBS];
int job_count = 0;
pthread_mutex_t job_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_available = PTHREAD_COND_INITIALIZER;
int scheduling_policy = 0; // 0: FCFS, 1: SJF, 2: Priority

void submit_job(char *name, int time, int priority) {
    pthread_mutex_lock(&job_queue_lock);
    if (job_count < MAX_JOBS) {
        strcpy(job_queue[job_count].name, name);
        job_queue[job_count].execution_time = time;
        job_queue[job_count].priority = priority;
        job_count++;
        printf("Job %s submitted (Exec Time: %d, Priority: %d)\n", name, time, priority);
        sort_jobs();
    }
    pthread_cond_signal(&job_available);
    pthread_mutex_unlock(&job_queue_lock);
}

void sort_jobs() {
    if (scheduling_policy == 1) { // SJF
        for (int i = 0; i < job_count - 1; i++) {
            for (int j = 0; j < job_count - i - 1; j++) {
                if (job_queue[j].execution_time > job_queue[j + 1].execution_time) {
                    Job temp = job_queue[j];
                    job_queue[j] = job_queue[j + 1];
                    job_queue[j + 1] = temp;
                }
            }
        }
    } else if (scheduling_policy == 2) { // Priority
        for (int i = 0; i < job_count - 1; i++) {
            for (int j = 0; j < job_count - i - 1; j++) {
                if (job_queue[j].priority < job_queue[j + 1].priority) {
                    Job temp = job_queue[j];
                    job_queue[j] = job_queue[j + 1];
                    job_queue[j + 1] = temp;
                }
            }
        }
    }
}

void change_scheduling_policy(int policy) {
    pthread_mutex_lock(&job_queue_lock);
    scheduling_policy = policy;
    sort_jobs();
    pthread_mutex_unlock(&job_queue_lock);
}
