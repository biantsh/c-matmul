// Standard matrix multiplication on a single core.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

# include "lib/mat_util.h"

void mat_mul(int size, int** matrix_1, int** matrix_2, int** result) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            result[row][col] = 0;
            for (int index = 0; index < size; index++) {
                result[row][col] += matrix_1[row][index] * matrix_2[index][col];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    long size = strtol(argv[1], NULL, 10);

    int** matrix_1 = mat_alloc(size);
    int** matrix_2 = mat_alloc(size);
    int** result = mat_alloc(size);

    mat_fill(matrix_1, size);
    mat_fill(matrix_2, size);
    mat_fill(result, size);

    clock_t time_start = clock();
    mat_mul(size, matrix_1, matrix_2, result);
    clock_t time_end = clock();

    double time_taken = (double) (time_end - time_start) / CLOCKS_PER_SEC;
    printf("Time taken: %.3f seconds\n", time_taken);
    printf("Matrix size: %ld", size);
}
