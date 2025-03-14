#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job.h"

Job job_queue[MAX_JOBS];
int job_count = 0;
int scheduling_policy = 0;  // Default: FCFS
int job_index = 0;          // Track executed jobs

pthread_mutex_t job_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_available = PTHREAD_COND_INITIALIZER;

int parse_scheduling_policy(char *policy_str) {
    if (strcmp(policy_str, "0") == 0 || strcmp(policy_str, "fcfs") == 0) return 0;
    if (strcmp(policy_str, "1") == 0 || strcmp(policy_str, "sjf") == 0) return 1;
    if (strcmp(policy_str, "2") == 0 || strcmp(policy_str, "priority") == 0) return 2;
    return -1;
}

void submit_job(char *name, int time, int priority) {
    pthread_mutex_lock(&job_queue_lock);

    if (job_count < MAX_JOBS) {
        strcpy(job_queue[job_count].name, name);
        job_queue[job_count].execution_time = time;
        job_queue[job_count].priority = priority;
        job_count++;

        printf("Job %s submitted (Exec Time: %d, Priority: %d)\n", name, time, priority);
        printf("DEBUG: Total jobs in queue after submission: %d\n", job_count); // Debugging output

        sort_jobs();
        pthread_cond_signal(&job_available);
    } else {
        printf("Job queue is full. Cannot add more jobs.\n");
    }

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

void submit_job_with_output(char *name, int execution_time, int priority) {
    submit_job(name, execution_time, priority);

    // Calculate expected waiting time (sum of all job execution times in queue)
    int waiting_time = 0;
    for (int i = 0; i < job_count - 1; i++) { // Exclude the latest job itself
        waiting_time += job_queue[i].execution_time;
    }

    printf("Job %s was submitted.\n", name);
    printf("Total number of jobs in the queue: %d\n", job_count);
    printf("Expected waiting time: %d seconds\n", waiting_time);
    printf("Scheduling Policy: %s\n", scheduling_policy == 0 ? "FCFS" : scheduling_policy == 1 ? "SJF" : "Priority");
}