#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, local_data, total_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_data = rank + 1; // Each process contributes its rank+1

    // Sum values into total_data on root process (rank 0)
    MPI_Reduce(&local_data, &total_data, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum of all data is: %d\n", total_data);
    }

    MPI_Finalize();
    return 0;
}

