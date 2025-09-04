#include <stdio.h>
#include <omp.h>

#define N 3   // You can change size or take as input

int main() {
    int A[N][N], B[N][N], C[N][N];
    int i, j, k;

    // Input matrices
    printf("Enter elements of Matrix A (%dx%d):\n", N, N);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of Matrix B (%dx%d):\n", N, N);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            scanf("%d", &B[i][j]);

    // Initialize result matrix
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = 0;

    double start, end;

    // Parallel multiplication with OpenMP
    start = omp_get_wtime();
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end = omp_get_wtime();

    // Print result
    printf("\nResultant Matrix C:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d\t", C[i][j]);
        }
        printf("\n");
    }

    printf("\nTime taken: %f seconds\n", end - start);

    return 0;
}

