#include <pthread.h>
#include "mat_util.h"

typedef struct {
    int** matrix_1;
    int** matrix_2;
    int** result;
    int size;
    int row_start;
    int row_end;
} ThreadArgs;

void* mat_mul_threaded(void* thread_args) {
    ThreadArgs* args = (ThreadArgs*) thread_args;

    for (int row = args->row_start; row < args->row_end; row++) {
        for (int col = 0; col < args->size; col++) {
            args->result[row][col] = 0;
            for (int index = 0; index < args->size; index++) {
                args->result[row][col] += args->matrix_1[row][index] * args->matrix_2[index][col];
            }
        }
    }

    pthread_exit(NULL);
    return NULL;
}

void mat_mul_standard(int** matrix_1, int** matrix_2, int** result, int size, int num_threads) {
    pthread_t threads[num_threads];
    ThreadArgs thread_args[num_threads];

    int rows_per_thread = size / num_threads;
    for (int thread_idx = 0; thread_idx < num_threads; thread_idx++) {
        thread_args[thread_idx].matrix_1 = matrix_1;
        thread_args[thread_idx].matrix_2 = matrix_2;
        thread_args[thread_idx].result = result;
        thread_args[thread_idx].size = size;
        thread_args[thread_idx].row_start = thread_idx * rows_per_thread;
        thread_args[thread_idx].row_end = (thread_idx + 1) * rows_per_thread;

        // Ensure last thread covers remaining rows
        if (thread_idx == num_threads - 1) {
            thread_args[thread_idx].row_end = size;
        }

        pthread_create(&threads[thread_idx], NULL, mat_mul_threaded, (void*)&thread_args[thread_idx]);
    }

    for (int thread_idx = 0; thread_idx < num_threads; thread_idx++) {
        pthread_join(threads[thread_idx], NULL);
    }
}

void mat_mul_strassen(int** matrix_1, int** matrix_2, int** result, int size, int num_threads, int leaf_size) {
    if (size <= leaf_size) {
        mat_mul_standard(matrix_1, matrix_1, result, size, num_threads);
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
    mat_mul_strassen(a11, temp_b, p1, new_size, num_threads, leaf_size);

    // p2 = (a11 + a12) * b22
    mat_add(a11, a12, temp_a, new_size);
    mat_mul_strassen(temp_a, b22, p2, new_size, num_threads, leaf_size);

    // p3 = (a21 + a22) * b11
    mat_add(a21, a22, temp_a, new_size);
    mat_mul_strassen(temp_a, b11, p3, new_size, num_threads, leaf_size);

    // p4 = a22 * (b21 - b11)
    mat_sub(b21, b11, temp_b, new_size);
    mat_mul_strassen(a22, temp_b, p4, new_size, num_threads, leaf_size);

    // p5 = (a11 + a22) * (b11 + b22)
    mat_add(a11, a22, temp_a, new_size);
    mat_add(b11, b22, temp_b, new_size);
    mat_mul_strassen(temp_a, temp_b, p5, new_size, num_threads, leaf_size);

    // p6 = (a12 - a22) * (b21 + b22)
    mat_sub(a12, a22, temp_a, new_size);
    mat_add(b21, b22, temp_b, new_size);
    mat_mul_strassen(temp_a, temp_b, p6, new_size, num_threads, leaf_size);

    // p7 = (a11 - a21) * (b11 + b12)
    mat_sub(a11, a21, temp_a, new_size);
    mat_add(b11, b12, temp_b, new_size);
    mat_mul_strassen(temp_a, temp_b, p7, new_size, num_threads, leaf_size);

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
