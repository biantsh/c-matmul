// Standard matrix multiplication on a single core.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 512

void mat_mul(int* matrix_1[SIZE], int* matrix_2[SIZE], int* result[SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            result[row][col] = 0;
            for (int index = 0; index < SIZE; index++) {
                result[row][col] += matrix_1[row][index] * matrix_2[index][col];
            }
        }
    }
}

int main() {
    int* matrix_1[SIZE];
    int* matrix_2[SIZE];
    int* result[SIZE];

    for (int row = 0; row < SIZE; row++) {
        matrix_1[row] = (int *) malloc(SIZE * sizeof(int));
        matrix_2[row] = (int *) malloc(SIZE * sizeof(int));
        result[row] = (int *) malloc(SIZE * sizeof(int));

        for (int col = 0; col < SIZE; col++) {
            matrix_1[row][col] = rand() % 10;
            matrix_2[row][col] = rand() % 10;
        }
    }

    clock_t time_start = clock();
    mat_mul(matrix_1, matrix_2, result);
    clock_t time_end = clock();

    double time_taken = (double) (time_end - time_start) / CLOCKS_PER_SEC;
    printf("Time taken: %.3f seconds\n", time_taken);
}
