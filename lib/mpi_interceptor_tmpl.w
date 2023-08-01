#include "InterceptorFunctions.h"

{{fn mpi_fun MPI_Init MPI_Init_thread}}
const void* wrapped_ret_addr = __builtin_return_address(0);
{{callfn}}
mpi_arg_trace_start("{{mpi_fun}}", wrapped_ret_addr);
{{endfn}}

{{fnall mpi_fun MPI_Finalize MPI_Init MPI_Init_thread MPI_Initialized
        MPI_Comm_dup MPI_Comm_dup_with_info MPI_Comm_idup MPI_Comm_create MPI_Comm_create_group MPI_Comm_split MPI_Comm_split_type
        MPI_Intercomm_create MPI_Intercomm_merge
        MPI_Dist_graph_create MPI_Dist_graph_create_adjacent
        MPI_Cart_create MPI_Cart_sub
        MPI_Graph_create}}
  const void* wrapped_ret_addr = __builtin_return_address(0);
  mpi_arg_trace_push("{{mpi_fun}}", wrapped_ret_addr);
  {{full_trace_callback}}
  {{callfn}}
{{endfnall}}

{{fn mpi_fun MPI_Comm_dup MPI_Comm_dup_with_info MPI_Comm_idup MPI_Comm_create MPI_Comm_create_group MPI_Comm_split MPI_Comm_split_type
          MPI_Intercomm_create MPI_Intercomm_merge
          MPI_Dist_graph_create MPI_Dist_graph_create_adjacent
          MPI_Cart_create MPI_Cart_sub
          MPI_Graph_create}}
const void* wrapped_ret_addr = __builtin_return_address(0);
mpi_arg_trace_push("{{mpi_fun}}", wrapped_ret_addr);
{{callfn}}
{{full_trace_callback}}
{{endfn}}

{{fn mpi_fun MPI_Finalize}}
const void* wrapped_ret_addr = __builtin_return_address(0);
mpi_arg_trace_exit("{{mpi_fun}}", wrapped_ret_addr);
{{callfn}}
{{endfn}}
