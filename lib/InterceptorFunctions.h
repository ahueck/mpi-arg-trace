#ifndef MPI_INTERCEPTOR_INTERCEPTORFUNCTIONS_H_
#define MPI_INTERCEPTOR_INTERCEPTORFUNCTIONS_H_

#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: expand for all possible MPI args
void mpi_arg_trace_push(const char* mpi_fn_name, const void* called_from);

// Called from finalize
void mpi_arg_trace_exit(const char* mpi_fn_name, const void* called_from);

#ifdef __cplusplus
}
#endif

#endif
