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
  int* ACCESS_MODE,
  MPI_Aint* ALLOC_MEM_NUM_BYTES,
  int* ARGUMENT_COUNT,
  char* ARGUMENT_LIST,
  int* ARRAY_LENGTH,
  int* ARRAY_LENGTH_NNI,
  int* ARRAY_LENGTH_PI,
  int* ASSERT,
  void* ATTRIBUTE_VAL,
  void* ATTRIBUTE_VAL_10,
  const void* BUFFER,
  int* COLOR,
  MPI_Comm* COMMUNICATOR,
  MPI_Comm* COMM_COMPARISON,
  int* COMM_SIZE,
  const int* COORDINATE,
  void* C_BUFFER,
  void* C_BUFFER2,
  MPI_Datatype* DATATYPE,
  int* DEGREE,
  int* DIMENSION,
  int* DISPLACEMENT,
  MPI_Errhandler* ERRHANDLER,
  int* ERROR_CLASS,
  int* ERROR_CODE,
  int* EXTRA_STATE,
  MPI_File* FILE,
  MPI_Win_copy_attr_function* FUNCTION,
  MPI_Group* GROUP,
  int* INDEX,
  MPI_Info* INFO,
  int* INFO_VALUE_LENGTH,
  int* KEY,
  int* KEYVAL,
  int* KEY_INDEX,
  int* LOCK_TYPE,
  int* LOGICAL,
  MPI_Message* MESSAGE,
  MPI_Count* NUM_BYTES,
  int* NUM_DIMS,
  MPI_Offset* OFFSET,
  MPI_Datatype* OPERATION,
  const int* ORDER,
  const int* POLYDISPLACEMENT,
  MPI_Aint* POLYDISPLACEMENT_AINT_COUNT,
  int* POLYDISPLACEMENT_COUNT,
  MPI_Aint* POLYDISPOFFSET,
  void* POLYDTYPE_NUM_ELEM,
  int* POLYDTYPE_NUM_ELEM_NNI,
  const int* POLYDTYPE_NUM_ELEM_PI,
  int* POLYDTYPE_STRIDE_BYTES,
  MPI_Datarep_conversion_function* POLYFUNCTION,
  int* POLYNUM_BYTES,
  int* POLYNUM_BYTES_NNI,
  int* POLYRMA_DISPLACEMENT,
  int* POLYXFER_NUM_ELEM,
  int* POLYXFER_NUM_ELEM_NNI,
  const int* PROFILE_LEVEL,
  int* RANK,
  int* RANK_NNI,
  MPI_Request* REQUEST,
  MPI_Aint* RMA_DISPLACEMENT_NNI,
  int* SPLIT_TYPE,
  MPI_Status* STATUS,
  const char* STRING,
  int* STRING_LENGTH,
  int* TAG,
  int* THREAD_LEVEL,
  int* TOPOLOGY_TYPE,
  int* TYPECLASS,
  int* TYPECLASS_SIZE,
  int* UPDATE_MODE,
  int* VERSION,
  int* WEIGHT,
  MPI_Win* WINDOW,
  MPI_Aint* WINDOW_SIZE,
  MPI_Aint* WIN_ATTACH_SIZE,
  MPI_Comm* newCOMMUNICATOR,
  MPI_Datatype* newDATATYPE,
  MPI_Group* newGROUP,
  MPI_Info* newINFO
);
// Called from finalize
void mpi_arg_trace_exit(const char* mpi_fn_name, const void* called_from);

#ifdef __cplusplus
}
#endif

#endif
