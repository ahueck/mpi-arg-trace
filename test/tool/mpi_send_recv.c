// RUN: OMPI_CC=clang %mpicc %s -g -O0 -o %s.exe
// RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 2 %s.exe 2>&1 | %filecheck %s

#include <mpi.h>

// CHECK: MPI_{{(Send|Recv)}},MPI_INT,MPI_COMM_WORLD

// CHECK: MPI_Type_contiguous,MPI_INT
// CHECK: MPI_Type_commit,MPI_Type_contiguous
// CHECK: MPI_Type_contiguous,MPI_Type_contiguous

// CHECK: MPI_{{(Send|Recv)}},MPI_Type_contiguous,MPI_COMM_WORLD

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

  MPI_Datatype customType = NULL;
  MPI_Type_contiguous(1, MPI_INT, &customType);
  MPI_Datatype nestedType = NULL;
  MPI_Type_commit(&customType);
  MPI_Type_contiguous(1, customType, &nestedType);

  if (rank == 0) {
    data = 42;
    MPI_Send(&data, 1, customType, 1, 0, MPI_COMM_WORLD);
  } else if (rank == 1) {
    MPI_Recv(&data, 1, customType, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  MPI_Type_free(&customType);

  MPI_Finalize();
}

int main(int argc, char** argv) {
  comm(argc, argv);
  return 0;
}
