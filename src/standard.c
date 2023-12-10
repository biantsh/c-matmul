// Standard matrix multiplication on a single core.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/mat_mul.h"
#include "lib/mat_util.h"

int main(int argc, char* argv[]) {
    long size = strtol(argv[1], NULL, 10);

    int** matrix_1 = mat_alloc(size);
    int** matrix_2 = mat_alloc(size);
    int** result = mat_alloc(size);

    mat_fill(matrix_1, size);
    mat_fill(matrix_2, size);
    mat_fill(result, size);

    clock_t time_start = clock();
    mat_mul_standard(matrix_1, matrix_2, result, size);
    clock_t time_end = clock();

    double time_taken = (double) (time_end - time_start) / CLOCKS_PER_SEC;
    printf("Time taken: %.3f seconds\n", time_taken);
    printf("Matrix size: %ld", size);
}
