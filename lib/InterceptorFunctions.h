#ifndef MPI_INTERCEPTOR_INTERCEPTORFUNCTIONS_H_
#define MPI_INTERCEPTOR_INTERCEPTORFUNCTIONS_H_

#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: expand for all possible MPI args
void mpi_arg_trace_push(const char* mpi_fn_name, const void* called_from);
void mpi_arg_trace_push_full(
  const char* mpi_fun_name,
  const void* called_from,
  const int* ACCESS_MODE,
  const MPI_Aint* ALLOC_MEM_NUM_BYTES,
  const int* ARGUMENT_COUNT,
  char*** ARGUMENT_LIST,
  const int* ARRAY_LENGTH,
  const int* ARRAY_LENGTH_NNI,
  const int* ARRAY_LENGTH_PI,
  const int* ASSERT,
  const void* ATTRIBUTE_VAL,
  const void* ATTRIBUTE_VAL_10,
  const void* BUFFER,
  const int* COLOR,
  const MPI_Comm* COMMUNICATOR,
  const int* COMM_COMPARISON,
  const int* COMM_SIZE,
  const int* COORDINATE,
  const void* C_BUFFER,
  const void* C_BUFFER2,
  const MPI_Datatype* DATATYPE,
  const int* DEGREE,
  const int* DIMENSION,
  const MPI_Aint* DISPLACEMENT,
  const MPI_Errhandler* ERRHANDLER,
  const int* ERROR_CLASS,
  const int* ERROR_CODE,
  const int* EXTRA_STATE,
  const MPI_File* FILE,
  //const MPI_Win_copy_attr_function* FUNCTION,//TODO MPI_Win_copy_attr_function is not the only type possible here!
  const void* FUNCTION,//TODO MPI_Win_copy_attr_function is not the only type possible here!
  const MPI_Group* GROUP,
  const int* INDEX,
  const MPI_Info* INFO,
  const int* INFO_VALUE_LENGTH,
  const int* KEY,
  const int* KEYVAL,
  const int* KEY_INDEX,
  const int* LOCK_TYPE,
  const int* LOGICAL,
  const MPI_Message* MESSAGE,
  const MPI_Count* NUM_BYTES,
  const int* NUM_DIMS,
  const MPI_Offset* OFFSET,
  const MPI_Op* OPERATION,
  const int* ORDER,
  const int* POLYDISPLACEMENT,
  const MPI_Aint* POLYDISPLACEMENT_AINT_COUNT,
  const int* POLYDISPLACEMENT_COUNT,
  const MPI_Aint* POLYDISPOFFSET,
  const void* POLYDTYPE_NUM_ELEM,
  const int* POLYDTYPE_NUM_ELEM_NNI,
  const int* POLYDTYPE_NUM_ELEM_PI,
  const MPI_Aint* POLYDTYPE_STRIDE_BYTES,
  //const MPI_Datarep_conversion_function* POLYFUNCTION,//TODO other types of functions are possible here
  const void* POLYFUNCTION,
  const int* POLYNUM_BYTES,
  const int* POLYNUM_BYTES_NNI,
  const int* POLYRMA_DISPLACEMENT,
  const int* POLYXFER_NUM_ELEM,
  const int* POLYXFER_NUM_ELEM_NNI,
  const int* PROFILE_LEVEL,
  const int* RANK,
  const int* RANK_NNI,
  const MPI_Request* REQUEST,
  const MPI_Aint* RMA_DISPLACEMENT_NNI,
  const int* SPLIT_TYPE,
  const MPI_Status* STATUS,
  const char* STRING,
  const int* STRING_LENGTH,
  const int* TAG,
  const int* THREAD_LEVEL,
  const int* TOPOLOGY_TYPE,
  const int* TYPECLASS,
  const int* TYPECLASS_SIZE,
  const int* UPDATE_MODE,
  const int* VERSION,
  const int* WEIGHT,
  const MPI_Win* WINDOW,
  const MPI_Aint* WINDOW_SIZE,
  const MPI_Aint* WIN_ATTACH_SIZE,
  const MPI_Comm* newCOMMUNICATOR,
  const MPI_Datatype* newDATATYPE,
  const MPI_Group* newGROUP,
  const MPI_Info* newINFO
);
// Called from finalize
void mpi_arg_trace_exit(const char* mpi_fn_name, const void* called_from);

#ifdef __cplusplus
}
#endif

#endif
