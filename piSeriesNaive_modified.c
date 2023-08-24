#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name) {
    fprintf(stderr, "Usage: %s <num_threads> <n> <schedule_type> [<block_size>]\n", prog_name);
    fprintf(stderr, "  schedule_type: static, dynamic, guided, auto\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    long long n, k;
    int thread_count;
    double sum = 0.0;
    double start_time, end_time;

    if (argc < 4) {
        Usage(argv[0]);
        return 1;
    }

    thread_count = strtol(argv[1], NULL, 10);
    n = strtoll(argv[2], NULL, 10);
    char* schedule_type = argv[3];

    int block_size = 128;
    if (argc == 5) {
        block_size = strtol(argv[4], NULL, 10);
    }

    printf("Threads = %d\n", thread_count);
    printf("Schedule Type = %s\n", schedule_type);
    if (strcmp(schedule_type, "auto") != 0) {
        printf("Block Size = %d\n", block_size);
    }

    start_time = omp_get_wtime();

    #pragma omp parallel for num_threads(thread_count) schedule(runtime)
    for (k = 0; k < n; k++) {
        double factor = (k % 2 == 0) ? 1.0 : -1.0;
        sum += factor / (2 * k + 1);
    }

    sum = 4.0 * sum;

    end_time = omp_get_wtime();

    printf("With n = %lld terms and %d threads,\n", n, thread_count);
    printf("   Our estimate of pi = %.14f\n", sum);
    printf("                   PI = %.14f\n", 4.0 * atan(1.0));

    printf("Time: %.6f seconds\n", end_time - start_time);

    return 0;
}
