// This code is repurposed from my aubatch.c file.
// I originally thought I understood the requirement for the test command to perform this task.
// This has been reused here and the test command edited to meet its actual requirement.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <exe_time>\n", argv[0]);
        return 1;
    }

    int exe_time = atoi(argv[1]);  // Convert argument to integer
    if (exe_time <= 0) {
        fprintf(stderr, "Error: Execution time must be a positive integer.\n");
        return 1;
    }

    printf("Batch job started. Running for %d seconds...\n", exe_time);
    sleep(exe_time);
    printf("Batch job completed.\n");

    return 0;
}
