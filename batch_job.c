/* All code in this source is my own. Primary source for code reference was Microsoft.learn.
* This code was based off of Dr. Qin's code. It is not the same but I did start from there.
* I did not copy/paste code directly from Phind AI but I did have it educate me and assist in solving my issues.
* COMP7500: Project 3: Aubatch
* Tucker Watts
* Auburn University
* This code is repurposed from my aubatch.c file.
* I originally thought I understood the requirement for the test command to perform this task.
* This has been reused here and the test command edited to meet its actual requirement.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <exe_time>\n", argv[0]);
        return 1;
    }

    double exe_time = atof(argv[1]);  // Convert argument to double for precision
    if (exe_time <= 0) {
        fprintf(stderr, "Error: Execution time must be a positive number.\n");
        return 1;
    }

    printf("Batch job started. Running for %.2f seconds...\n", exe_time);

    // Open log file
    FILE *log_file = fopen("batch_job.log", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return 1;
    }

    fprintf(log_file, "Batch job started. Estimated duration: %.2f seconds\n", exe_time);

    // Simulate CPU work
    double result = 0.0;
    clock_t start_time = clock();
    while ((double)(clock() - start_time) / CLOCKS_PER_SEC < exe_time) {
        for (int i = 0; i < 1000000; i++) {
            result += sin(i) * cos(i);  // Dummy computation
        }
        fprintf(log_file, "Progress: %.2f%%\n", ((double)(clock() - start_time) / CLOCKS_PER_SEC / exe_time) * 100);
    }

    fprintf(log_file, "Batch job completed. Result: %.5f\n", result);
    fclose(log_file);

    printf("Batch job completed.\n");
    return 0;
}
