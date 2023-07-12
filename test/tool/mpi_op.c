// RUN: OMPI_CC=clang %mpicc %s -g -O0 -o %s.exe
// RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 1 %s.exe 2>&1 | %filecheck %s

#include <mpi.h>

// CHECK: MPI_Allreduce,,,1,MPI_INT,MPI_COMM_WORLD,MPI_SUM

void comm(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int data             = 0;
  int reduction_result = 0;
  MPI_Allreduce(&data, &reduction_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  MPI_Finalize();
}

int main(int argc, char** argv) {
  comm(argc, argv);
  return 0;
}
