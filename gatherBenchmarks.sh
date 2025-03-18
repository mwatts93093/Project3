#!/bin/bash

# Define test parameters
job_counts=(5 10 15 20 25)
arrival_rates=(0.1 0.2 0.5 1.0)
cpu_time_ranges=("0.1 0.5" "0.1 1" "0.5 1" "1 10")
scheduling_policies=("fcfs" "sjf" "priority")
benchmark_name="mybenchmark"

# Create a timestamped log file
log_file="aubatch_test_results_$(date +%F_%T).log"
echo "AUbatch Performance Test Log - $(date)" > "$log_file"
echo "=========================================" >> "$log_file"

# Start automated testing
echo "Starting automated testing..."
for policy in "${scheduling_policies[@]}"; do
    for jobs in "${job_counts[@]}"; do
        for arrival in "${arrival_rates[@]}"; do
            for range in "${cpu_time_ranges[@]}"; do
                min_time=$(echo "$range" | awk '{print $1}')
                max_time=$(echo "$range" | awk '{print $2}')

                # Generate the test command
                test_command="test $benchmark_name $policy $jobs $arrival 5 $min_time $max_time"

                # Log the command being executed
                echo "Running: $test_command" | tee -a "$log_file"

                # Run AUbatch non-interactively and immediately exit
                echo -e "$test_command\nquit" | ./aubatch >> "$log_file" 2>&1
                
                echo "--------------------------------------" >> "$log_file"
            done
        done
    done
done

echo "Tests completed! Results saved in: $log_file"
