// RUN: OMPI_CC=clang %mpicc %s -g -O0 -o %s.exe
// RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 1 %s.exe 2>&1 | %filecheck %s

#include <mpi.h>

// CHECK: MPI_Comm_dup,,,,,MPI_COMM_WORLD
// CHECK: MPI_Bcast,0,,1,MPI_INT,MPI_Comm_dup,,,,{{.*}}/mpi_comm.c,comm,19

// CHECK: MPI_Comm_dup,,,,,MPI_Comm_dup
// CHECK: MPI_Bcast,0,,1,MPI_INT,MPI_Comm_dup,,,,{{.*}}/mpi_comm.c,comm,23

void comm(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int data = 0;

  MPI_Comm duplicated_communicator;
  MPI_Comm_dup(MPI_COMM_WORLD, &duplicated_communicator);
  MPI_Bcast(&data, 1, MPI_INT, 0, duplicated_communicator);

  MPI_Comm duplicated_communicator_nested;
  MPI_Comm_dup(duplicated_communicator, &duplicated_communicator_nested);
  MPI_Bcast(&data, 1, MPI_INT, 0, duplicated_communicator_nested);

  MPI_Finalize();
}

int main(int argc, char** argv) {
  comm(argc, argv);
  return 0;
}
