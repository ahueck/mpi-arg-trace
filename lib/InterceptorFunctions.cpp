#include "InterceptorFunctions.h"

#include <iostream>

void mpi_arg_trace_push(const char* mpi_fn_name, const void* called_from) {
  std::cerr << mpi_fn_name << "\n";
}

void mpi_arg_trace_exit(const char* mpi_fn_name, const void* called_from) {
  std::cerr << "MPI exit: " << mpi_fn_name << "\n";
}
