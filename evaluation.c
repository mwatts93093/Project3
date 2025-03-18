#include <stdio.h>
#include "job.h"  // This now includes the function declarations for sorting

void evaluate_performance(Job jobs[], int num_jobs, int scheduling_policy) {
    double total_turnaround_time = 0;
    double total_waiting_time = 0;
    double total_cpu_time = 0;

    if (scheduling_policy == FCFS) {
        double completion_time = 0;
        for (int i = 0; i < num_jobs; i++) {
            total_waiting_time += completion_time;
            completion_time += jobs[i].execution_time;
            total_turnaround_time += completion_time;
            total_cpu_time += jobs[i].execution_time;
        }
    } 
    else if (scheduling_policy == SJF) {
        sort_jobs_by_execution_time(jobs, num_jobs);  // Now properly declared & implemented

        double completion_time = 0;
        for (int i = 0; i < num_jobs; i++) {
            total_waiting_time += completion_time;
            completion_time += jobs[i].execution_time;
            total_turnaround_time += completion_time;
            total_cpu_time += jobs[i].execution_time;
        }
    } 
    else if (scheduling_policy == Priority) {
        sort_jobs_by_priority(jobs, num_jobs);  // Now properly declared & implemented

        double completion_time = 0;
        for (int i = 0; i < num_jobs; i++) {
            total_waiting_time += completion_time;
            completion_time += jobs[i].execution_time;
            total_turnaround_time += completion_time;
            total_cpu_time += jobs[i].execution_time;
        }
    }

    double avg_turnaround_time = total_turnaround_time / num_jobs;
    double avg_waiting_time = total_waiting_time / num_jobs;
    double avg_cpu_time = total_cpu_time / num_jobs;
    double throughput = num_jobs / total_turnaround_time;

    printf("=================================\n");
    printf("        Benchmark Results        \n");
    printf("=================================\n");
    printf("Total Jobs Submitted: %d\n", num_jobs);
    printf("Average Turnaround Time: %.2f seconds\n", avg_turnaround_time);
    printf("Average CPU Time: %.2f seconds\n", avg_cpu_time);
    printf("Average Waiting Time: %.2f seconds\n", avg_waiting_time);
    printf("Throughput: %.3f No./second\n", throughput);
    printf("=================================\n");
}
