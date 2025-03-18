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
#include <string.h>
#include "job.h"

Job job_queue[MAX_JOBS];
Job last_running_job;
int has_running_job = 0;  // Flag to indicate if a job is currently running
Job completed_jobs[MAX_COMPLETED];
int completed_count = 0;
int job_count = 0;
int scheduling_policy = 0;  // Default: FCFS
int job_index = 0;          // Track executed jobs
double response_times[MAX_COMPLETED] = {0};  // Initialize response times array

pthread_mutex_t job_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_available = PTHREAD_COND_INITIALIZER;

int parse_scheduling_policy(char *policy_str) {
    if (strcmp(policy_str, "0") == 0 || strcmp(policy_str, "fcfs") == 0) return 0;
    if (strcmp(policy_str, "1") == 0 || strcmp(policy_str, "sjf") == 0) return 1;
    if (strcmp(policy_str, "2") == 0 || strcmp(policy_str, "priority") == 0) return 2;
    return -1;
}

void sort_jobs() {
    if (scheduling_policy == 1) { // SJF
        for (int i = 0; i < job_count - 1; i++) {
            for (int j = 0; j < job_count - i - 1; j++) {
                if ((double)job_queue[j].execution_time > (double)job_queue[j + 1].execution_time) {
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

// Sorts by FCFS by default
void submit_job(char *name, double execution_time, int priority, int show_output) {
    pthread_mutex_lock(&job_queue_lock);

    if (job_count < MAX_JOBS) {
        int index = job_count;
        strcpy(job_queue[index].name, name);
        job_queue[index].execution_time = execution_time;
        job_queue[index].priority = priority;
        job_queue[index].submission_time = time(NULL);  // Capture submission time
        job_count++;

        pthread_cond_signal(&job_available);
    } else {
        printf("Job queue is full. Cannot add more jobs.\n");
    }

    pthread_mutex_unlock(&job_queue_lock);
}


// Function to sort jobs by execution time (Shortest Job First)
void sort_jobs_by_execution_time(Job jobs[], int num_jobs) {
    for (int i = 0; i < num_jobs - 1; i++) {
        for (int j = i + 1; j < num_jobs; j++) {
            if (jobs[i].execution_time > jobs[j].execution_time) {
                Job temp = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = temp;
            }
        }
    }
}

// Function to sort jobs by priority (Higher priority first)
void sort_jobs_by_priority(Job jobs[], int num_jobs) {
    for (int i = 0; i < num_jobs - 1; i++) {
        for (int j = i + 1; j < num_jobs; j++) {
            if (jobs[i].priority > jobs[j].priority) { 
                Job temp = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = temp;
            }
        }
    }
}