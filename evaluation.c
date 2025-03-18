#include <stdio.h>
#include "job.h"

void evaluate_performance(Job jobs[], int num_jobs, int scheduling_policy) {
    double total_turnaround_time = 0.0;
    double total_waiting_time = 0.0;
    double total_cpu_time = 0.0;

    printf("\n[DEBUG] Evaluating Performance for %d Jobs (Policy: %d)\n", num_jobs, scheduling_policy);

    if (scheduling_policy == FCFS) {
        double completion_time = 0.0;
        for (int i = 0; i < num_jobs; i++) {
            printf("[DEBUG] Job %d Execution Time: %.2f\n", i + 1, jobs[i].execution_time);
            
            total_waiting_time += completion_time;
            completion_time += jobs[i].execution_time;
            total_turnaround_time += completion_time;
            total_cpu_time += jobs[i].execution_time;

            printf("[DEBUG] Job %d Waiting Time: %.2f, Turnaround Time: %.2f\n", i + 1, total_waiting_time, total_turnaround_time);
        }
    } 
    else if (scheduling_policy == SJF) {
        sort_jobs_by_execution_time(jobs, num_jobs);

        double completion_time = 0.0;
        for (int i = 0; i < num_jobs; i++) {
            printf("[DEBUG] Job %d Execution Time: %.2f\n", i + 1, jobs[i].execution_time);

            total_waiting_time += completion_time;
            completion_time += jobs[i].execution_time;
            total_turnaround_time += completion_time;
            total_cpu_time += jobs[i].execution_time;

            printf("[DEBUG] Job %d Waiting Time: %.2f, Turnaround Time: %.2f\n", i + 1, total_waiting_time, total_turnaround_time);
        }
    } 
    else if (scheduling_policy == PRIORITY) {
        sort_jobs_by_priority(jobs, num_jobs);

        double completion_time = 0.0;
        for (int i = 0; i < num_jobs; i++) {
            printf("[DEBUG] Job %d Execution Time: %.2f\n", i + 1, jobs[i].execution_time);

            total_waiting_time += completion_time;
            completion_time += jobs[i].execution_time;
            total_turnaround_time += completion_time;
            total_cpu_time += jobs[i].execution_time;

            printf("[DEBUG] Job %d Waiting Time: %.2f, Turnaround Time: %.2f\n", i + 1, total_waiting_time, total_turnaround_time);
        }
    }

    double avg_turnaround_time = (num_jobs > 0) ? total_turnaround_time / num_jobs : 0.0;
    double avg_waiting_time = (num_jobs > 0) ? total_waiting_time / num_jobs : 0.0;
    double avg_cpu_time = (num_jobs > 0) ? total_cpu_time / num_jobs : 0.0;
    double throughput = (total_turnaround_time > 0) ? num_jobs / total_turnaround_time : 0.0;

    printf("\n=================================\n");
    printf("        Benchmark Results        \n");
    printf("=================================\n");
    printf("Total Jobs Submitted: %d\n", num_jobs);
    printf("Average Turnaround Time: %.2f seconds\n", avg_turnaround_time);
    printf("Average CPU Time: %.2f seconds\n", avg_cpu_time);
    printf("Average Waiting Time: %.2f seconds\n", avg_waiting_time);
    printf("Throughput: %.3f No./second\n", throughput);
    printf("=================================\n");
}