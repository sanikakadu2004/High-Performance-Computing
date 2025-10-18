#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, n = 1000000, i;
    double h, x, local_sum = 0.0, pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    h = 1.0 / (double) n;

    // Each process computes its portion of the integral
    for (i = rank; i < n; i += size) {
        x = h * (i + 0.5);
        local_sum += 4.0 / (1.0 + x * x);
    }
    local_sum *= h;

    // Combine all partial sums at the root process
    MPI_Reduce(&local_sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Calculated value of Pi = %.16f\n", pi);
    }

    MPI_Finalize();
    return 0;
}

