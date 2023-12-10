// Strassen algorithm for matrix multiplication.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/mat_util.h"

void mat_mul(int size, int** matrix_1, int** matrix_2, int** result) {
    if (size == 1) {
        result[0][0] = matrix_1[0][0] * matrix_2[0][0];
        return;
    }

    int new_size = size / 2;
    int** a11 = mat_alloc(new_size);
    int** a12 = mat_alloc(new_size);
    int** a21 = mat_alloc(new_size);
    int** a22 = mat_alloc(new_size);
    int** b11 = mat_alloc(new_size);
    int** b12 = mat_alloc(new_size);
    int** b21 = mat_alloc(new_size);
    int** b22 = mat_alloc(new_size);

    int** c11 = mat_alloc(new_size);
    int** c12 = mat_alloc(new_size);
    int** c21 = mat_alloc(new_size);
    int** c22 = mat_alloc(new_size);

    int** p1 = mat_alloc(new_size);
    int** p2 = mat_alloc(new_size);
    int** p3 = mat_alloc(new_size);
    int** p4 = mat_alloc(new_size);
    int** p5 = mat_alloc(new_size);
    int** p6 = mat_alloc(new_size);
    int** p7 = mat_alloc(new_size);

    int** temp_a = mat_alloc(new_size);
    int** temp_b = mat_alloc(new_size);

    // Divide the matrices into 4 sub-matrices
    for (int row = 0; row < new_size; row++) {
        for (int col = 0; col < new_size; col++) {
            a11[row][col] = matrix_1[row][col];
            a12[row][col] = matrix_1[row][col + new_size];
            a21[row][col] = matrix_1[row + new_size][col];
            a22[row][col] = matrix_1[row + new_size][col + new_size];

            b11[row][col] = matrix_2[row][col];
            b12[row][col] = matrix_2[row][col + new_size];
            b21[row][col] = matrix_2[row + new_size][col];
            b22[row][col] = matrix_2[row + new_size][col + new_size];
        }
    }

    // p1 = a11 * (b12 - b22)
    mat_sub(b12, b22, temp_b, new_size);
    mat_mul(new_size, a11, temp_b, p1);

    // p2 = (a11 + a12) * b22
    mat_add(a11, a12, temp_a, new_size);
    mat_mul(new_size, temp_a, b22, p2);

    // p3 = (a21 + a22) * b11
    mat_add(a21, a22, temp_a, new_size);
    mat_mul(new_size, temp_a, b11, p3);

    // p4 = a22 * (b21 - b11)
    mat_sub(b21, b11, temp_b, new_size);
    mat_mul(new_size, a22, temp_b, p4);

    // p5 = (a11 + a22) * (b11 + b22)
    mat_add(a11, a22, temp_a, new_size);
    mat_add(b11, b22, temp_b, new_size);
    mat_mul(new_size, temp_a, temp_b, p5);

    // p6 = (a12 - a22) * (b21 + b22)
    mat_sub(a12, a22, temp_a, new_size);
    mat_add(b21, b22, temp_b, new_size);
    mat_mul(new_size, temp_a, temp_b, p6);

    // p7 = (a11 - a21) * (b11 + b12)
    mat_sub(a11, a21, temp_a, new_size);
    mat_add(b11, b12, temp_b, new_size);
    mat_mul(new_size, temp_a, temp_b, p7);

    // Construct the result matrix
    // c11 = p5 + p4 - p2 + p6
    mat_add(p5, p4, temp_a, new_size);
    mat_sub(temp_a, p2, temp_b, new_size);
    mat_add(temp_b, p6, c11, new_size);

    // c12 = p1 + p2
    mat_add(p1, p2, c12, new_size);

    // c21 = p3 + p4
    mat_add(p3, p4, c21, new_size);

    // c22 = p5 + p1 - p3 - p7
    mat_add(p5, p1, temp_a, new_size);
    mat_sub(temp_a, p3, temp_b, new_size);
    mat_sub(temp_b, p7, c22, new_size);

    // Assemble the final matrix
    for (int row = 0; row < new_size; row++) {
        for (int col = 0; col < new_size; col++) {
            result[row][col] = c11[row][col];
            result[row][col + new_size] = c12[row][col];
            result[row + new_size][col] = c21[row][col];
            result[row + new_size][col + new_size] = c22[row][col];
        }
    }

    mat_free(a11, new_size);
    mat_free(a12, new_size);
    mat_free(a21, new_size);
    mat_free(a22, new_size);
    mat_free(b11, new_size);
    mat_free(b12, new_size);
    mat_free(b21, new_size);
    mat_free(b22, new_size);

    mat_free(c11, new_size);
    mat_free(c12, new_size);
    mat_free(c21, new_size);
    mat_free(c22, new_size);

    mat_free(p1, new_size);
    mat_free(p2, new_size);
    mat_free(p3, new_size);
    mat_free(p4, new_size);
    mat_free(p5, new_size);
    mat_free(p6, new_size);
    mat_free(p7, new_size);

    mat_free(temp_a, new_size);
    mat_free(temp_b, new_size);
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
