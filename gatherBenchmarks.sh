# All code in this source is my own. Primary source for code reference was Microsoft.learn.
# This code was based off of Dr. Qin's code. It is not the same but I did start from there.
# I did not copy/paste code directly from Phind AI but I did have it educate me and assist in solving my issues.
# COMP7500: Project 3: Aubatch
# Tucker Watts
# Auburn University
# This script was made by me for the purpose of automating my benchmark gathering using the tool.

#!/bin/bash

# Define test parameters
job_counts=(5 10 15 20 25)
arrival_rates=(0.1 0.2 0.5 1.0)
cpu_time_ranges=("0.1 0.5" "0.1 1" "0.5 1" "1 10")
scheduling_policies=("fcfs" "sjf" "priority")
benchmark_name="mybenchmark"

# CSV Output File
csv_file="benchmark_results.csv"

# Write CSV headers
echo "Policy,Jobs,Arrival Rate (No./Sec),CPU Time Range,Turnaround Time,CPU Time,Waiting Time,Throughput" > "$csv_file"

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

                # Run the test command in AUbatch and capture output
                output=$(echo -e "$test_command\nquit" | ./aubatch)

                # Extract relevant metrics from output
                turnaround_time=$(echo "$output" | grep "Average Turnaround Time" | awk '{print $4}')
                cpu_time=$(echo "$output" | grep "Average CPU Time" | awk '{print $4}')
                waiting_time=$(echo "$output" | grep "Average Waiting Time" | awk '{print $4}')
                throughput=$(echo "$output" | grep "Throughput" | awk '{print $2}')

                # Append results to CSV
                echo "$policy,$jobs,$arrival,$min_time-$max_time,$turnaround_time,$cpu_time,$waiting_time,$throughput" >> "$csv_file"
            done
        done
    done
done

echo "Tests completed! Results saved in: $csv_file"
