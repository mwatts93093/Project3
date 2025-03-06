#include <stdio.h>
#include <unistd.h>
#include "job.h"

void execute_job(Job job) {
    printf("Executing job %s for %d seconds (Priority: %d)\n", job.name, job.execution_time, job.priority);
    sleep(job.execution_time);
    printf("Job %s completed.\n", job.name);
}
