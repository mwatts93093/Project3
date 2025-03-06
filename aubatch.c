#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "job.h"

void *scheduler_thread(void *arg);
void *dispatcher_thread(void *arg);

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

        Job job = job_queue[0];
        for (int i = 0; i < job_count - 1; i++) {
            job_queue[i] = job_queue[i + 1];
        }
        job_count--;

        pthread_mutex_unlock(&job_queue_lock);
        execute_job(job);
    }
    return NULL;
}
