#include "InterceptorFunctions.h"

#include "System.h"

#include <iostream>

void mpi_arg_trace_push(const char* mpi_fn_name, const void* called_from) {
  auto sloc = mpitracer::SourceLocation::create(called_from);

  if (sloc.has_value()) {
    std::cerr << mpi_fn_name << " " << sloc.value().file << " " << sloc.value().function << ":" << sloc.value().line
              << "\n";
  } else {
    std::cerr << mpi_fn_name << "\n";
  }
}

void mpi_arg_trace_exit(const char* mpi_fn_name, const void* called_from) {
  auto sloc = mpitracer::SourceLocation::create(called_from);
  if (sloc.has_value()) {
    std::cerr << mpi_fn_name << " " << sloc.value().file << " " << sloc.value().function << ":" << sloc.value().line
              << "\n";
  } else {
    std::cerr << mpi_fn_name << "\n";
  }
}
