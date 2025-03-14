#ifndef JOB_H
#define JOB_H

#include <pthread.h>

#define MAX_JOBS 10

typedef struct {
    char name[50];
    int execution_time;
    int priority;
} Job;

extern Job job_queue[MAX_JOBS];
extern int job_count;
extern int scheduling_policy;
extern int job_index;
extern pthread_mutex_t job_queue_lock;
extern pthread_cond_t job_available;

void submit_job(char *name, int time, int priority);
void execute_job(Job job);
void sort_jobs();
void change_scheduling_policy(int policy);
void submit_job_with_output(char *name, int execution_time, int priority);
void *dispatcher_thread(void *arg);

int parse_scheduling_policy(char *policy_str);

#endif
