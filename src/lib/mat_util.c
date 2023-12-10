// Utilities for working with NxN matrices.

#include <stdlib.h>

int** mat_alloc(int size) {
    int** matrix = (int **) malloc(size * sizeof(int*));

    for (int row = 0; row < size; row++) {
        matrix[row] = (int *) malloc(size * sizeof(int));
    }

    return matrix;
}

void mat_free(int** matrix, int size) {
    for (int row = 0; row < size; row++) {
        free(matrix[row]);
    }
    free(matrix);
}

void mat_fill(int** matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            matrix[row][col] = rand() % 10;
        }
    }
}

int** mat_add(int** matrix_1, int** matrix_2, int** result, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            result[row][col] = matrix_1[row][col] + matrix_2[row][col];
        }
    }

    return result;
}

int** mat_sub(int** matrix_1, int** matrix_2, int** result, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            result[row][col] = matrix_1[row][col] - matrix_2[row][col];
        }
    }

    return result;
}
