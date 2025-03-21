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

#ifndef JOB_H
#define JOB_H

#include <pthread.h>

#define MAX_JOBS 10
#define MAX_COMPLETED 100  // Set a limit on how many completed jobs to track

#define FCFS 0
#define SJF 1
#define Priority 2

typedef struct {
    char name[50];
    double execution_time;
    int priority;
    time_t submission_time;
    time_t start_time;
} Job;

extern Job job_queue[MAX_JOBS];
extern int job_count;
extern Job completed_jobs[MAX_COMPLETED];
extern int completed_count;
extern double response_times[MAX_COMPLETED];
extern int scheduling_policy;
extern int job_index;
extern pthread_mutex_t job_queue_lock;
extern pthread_cond_t job_available;
extern Job last_running_job;  // Stores the currently running job
extern int has_running_job;   // 1 if a job is running, 0 otherwise

void sort_jobs_by_execution_time(Job jobs[], int num_jobs);
void sort_jobs_by_priority(Job jobs[], int num_jobs);
void execute_job(Job job);
void sort_jobs();
void change_scheduling_policy(int policy);
void *dispatcher_thread(void *arg);
void submit_job(char *name, double execution_time, int priority, int show_output);
void evaluate_performance(Job jobs[], int num_jobs, int scheduling_policy);

int parse_scheduling_policy(char *policy_str);

#endif
