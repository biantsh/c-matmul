// Strassen algorithm for matrix multiplication.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/mat_mul.h"
#include "lib/mat_util.h"

int main(int argc, char* argv[]) {
    long mat_size = strtol(argv[1], NULL, 10);
    long leaf_size = strtol(argv[2], NULL, 10);

    int** matrix_1 = mat_alloc(mat_size);
    int** matrix_2 = mat_alloc(mat_size);
    int** result = mat_alloc(mat_size);

    mat_fill(matrix_1, mat_size);
    mat_fill(matrix_2, mat_size);
    mat_fill(result, mat_size);

    clock_t time_start = clock();
    mat_mul_strassen(matrix_1, matrix_2, result, mat_size, leaf_size);
    clock_t time_end = clock();

    double time_taken = (double) (time_end - time_start) / CLOCKS_PER_SEC;
    printf("Time taken: %.3f seconds\n", time_taken);
    printf("Matrix size: %ld", mat_size);
}
