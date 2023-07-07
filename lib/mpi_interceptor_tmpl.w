#include "InterceptorFunctions.h"

{{fn mpi_fun MPI_Init MPI_Init_Thread}}
const void* wrapped_ret_addr = __builtin_return_address(0);
mpi_arg_trace_start("{{mpi_fun}}", wrapped_ret_addr);
{{callfn}}
{{endfn}}

{{fnall mpi_fun MPI_Finalize MPI_Init MPI_Initialized}}
  const void* wrapped_ret_addr = __builtin_return_address(0);
  mpi_arg_trace_push("{{mpi_fun}}", wrapped_ret_addr);
  {{full_trace_callback}}
  {{callfn}}
{{endfnall}}

{{fn mpi_fun MPI_Finalize}}
const void* wrapped_ret_addr = __builtin_return_address(0);
mpi_arg_trace_exit("{{mpi_fun}}", wrapped_ret_addr);
{{callfn}}
{{endfn}}
