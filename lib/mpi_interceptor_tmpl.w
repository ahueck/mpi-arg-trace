#include "InterceptorFunctions.h"

{{fn mpi_fun MPI_Send MPI_Recv}}
  const void* wrapped_ret_addr = __builtin_return_address(0);
  mpi_arg_trace_push("{{mpi_fun}}", wrapped_ret_addr);
  {{callfn}}
{{endfn}}

{{fn mpi_fun MPI_Finalize}}
const void* wrapped_ret_addr = __builtin_return_address(0);
mpi_arg_trace_exit("{{mpi_fun}}", wrapped_ret_addr);
{{callfn}}
{{endfn}}