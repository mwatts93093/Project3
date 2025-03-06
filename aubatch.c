#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_JOBS 10

typedef struct {
    char name[50];
    int execution_time;
    int priority;
} Job;

Job job_queue[MAX_JOBS];
int job_count = 0;
int job_index = 0;
int scheduling_policy = 0; // 0: FCFS, 1: SJF, 2: Priority

pthread_mutex_t job_queue_lock;
pthread_cond_t job_available;

void *scheduler_thread(void *arg);
void *dispatcher_thread(void *arg);
void submit_job(char *name, int time, int priority);
void execute_job(Job job);
void sort_jobs();
void change_scheduling_policy(int policy);

int main() {
    pthread_t scheduler, dispatcher;
    
    pthread_mutex_init(&job_queue_lock, NULL);
    pthread_cond_init(&job_available, NULL);

    pthread_create(&scheduler, NULL, scheduler_thread, NULL);
    pthread_create(&dispatcher, NULL, dispatcher_thread, NULL);

    pthread_join(scheduler, NULL);
    pthread_join(dispatcher, NULL);

    pthread_mutex_destroy(&job_queue_lock);
    pthread_cond_destroy(&job_available);

    return 0;
}

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

void *scheduler_thread(void *arg) {
    char *jobs[] = {"job1", "job2", "job3"};
    int exec_times[] = {5, 3, 7};
    int priorities[] = {2, 1, 3};
    
    for (int i = 0; i < 3; i++) {
        sleep(1);
        submit_job(jobs[i], exec_times[i], priorities[i]);
    }
    return NULL;
}

void *dispatcher_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&job_queue_lock);
        while (job_count == 0) {
            pthread_cond_wait(&job_available, &job_queue_lock);
        }

        Job job = job_queue[job_index];
        job_index++;
        job_count--;
        pthread_mutex_unlock(&job_queue_lock);

        execute_job(job);
    }
    return NULL;
}

void execute_job(Job job) {
    printf("Executing job %s for %d seconds (Priority: %d)\n", job.name, job.execution_time, job.priority);
    sleep(job.execution_time);
    printf("Job %s completed.\n", job.name);
}
