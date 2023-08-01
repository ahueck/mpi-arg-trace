// RUN: OMPI_CC=clang %mpicc %s -g -O0 -o %s.exe
// RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 1 %s.exe 2>&1 | %filecheck %s

// CHECK: MPI_Type_contiguous,,,,MPI_INT,,,,MPI_Type_contiguous
// CHECK: MPI_Type_commit,,,,MPI_Type_contiguous,,,,,
// CHECK: MPI_Type_free,,,,MPI_Type_contiguous,,,,,

#include "mpi.h"

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  MPI_Datatype contig_2ints;
  MPI_Type_contiguous(2, MPI_INT, &contig_2ints);
  MPI_Type_commit(&contig_2ints);
  MPI_Type_free(&contig_2ints);

  MPI_Finalize();
}
