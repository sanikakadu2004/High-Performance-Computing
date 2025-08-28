#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matmul(int N, double *A, double *B, double *C) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i*N + k] * B[k*N + j];
            C[i*N + j] = sum;
        }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s matrix_size\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    double *A = malloc(N * N * sizeof(double));
    double *B = malloc(N * N * sizeof(double));
    double *C = malloc(N * N * sizeof(double));

    for (int i = 0; i < N*N; i++) {
        A[i] = 1.0;
        B[i] = 2.0;
    }

    clock_t start = clock();
    matmul(N, A, B, C);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Serial MatMul elapsed time: %f seconds\n", time_spent);

    free(A);
    free(B);
    free(C);

    return 0;
}

