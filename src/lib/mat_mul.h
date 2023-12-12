void mat_mul_standard(int** matrix_1, int** matrix_2, int** result, int size, int num_threads);

void mat_mul_strassen(int** matrix_1, int** matrix_2, int** result, int size, int num_threads, int leaf_size);
