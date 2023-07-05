// RUN: OMPI_CC=clang %mpicc %s -g -O0 -o %s.exe
// RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 2 %s.exe

#include <mpi.h>

void comm(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2) {
    MPI_Finalize();
    return;
  }

  int data = 0;
  if (rank == 0) {
    data = 42;
    MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (rank == 1) {
    MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  MPI_Finalize();
}

int main(int argc, char** argv) {
  comm(argc,argv);
  return 0;
}
