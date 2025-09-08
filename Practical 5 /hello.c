#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank (ID) of this process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Print message from each process
    printf("Hello from process %d of %d\n", rank, size);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}

