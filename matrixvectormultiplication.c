#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define N 8 // Size of matrix and vector

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;
    double A[rows_per_proc][N];
    double x[N];
    double y_local[rows_per_proc];
    double y[N];

    // Initialize vector x
    if(rank == 0) {
        for(int i = 0; i < N; i++)
            x[i] = i + 1;
    }

    // Broadcast vector x to all processes
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Initialize local portion of matrix A
    for(int i = 0; i < rows_per_proc; i++)
        for(int j = 0; j < N; j++)
            A[i][j] = (rank * rows_per_proc + i + 1) * (j + 1);

    // Parallel computation using OpenMP
    #pragma omp parallel for
    for(int i = 0; i < rows_per_proc; i++) {
        y_local[i] = 0.0;
        for(int j = 0; j < N; j++)
            y_local[i] += A[i][j] * x[j];
    }

    // Gather results at root process
    MPI_Gather(y_local, rows_per_proc, MPI_DOUBLE, y, rows_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print result in root process
    if(rank == 0) {
        printf("Result vector y:\n");
        for(int i = 0; i < N; i++)
            printf("%lf ", y[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

