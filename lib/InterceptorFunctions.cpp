#include "InterceptorFunctions.h"

#include "MPIUtil.h"
#include "System.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace mpitracer;
namespace fs = std::filesystem;

int mpi_trace_process_rank{0};

std::string mpi_trace_target_file{""};

struct StringTrace {
  std::ofstream trace_file;

  void open(int rank) {
    const auto env_file = getenv("MPI_ARG_TRACE_DIR");
    const auto file     = [](const auto* env_file) {
      fs::path path;
      if (env_file != nullptr) {
        path += env_file;
        if (!is_directory(path)) {
          std::cerr << "[MPI_ARG_TRACE] ERROR. Expected directory for MPI_ARG_TRACE_DIR. Given: '" << path << "'"
                    << std::endl;
          PMPI_Abort(MPI_COMM_WORLD, -1);
        }
      }
      path /= ("mpi-arg-trace-" + std::to_string(mpi_trace_process_rank) + ".csv");
      return path;
    }(env_file);

    trace_file.open(file, std::ios::out | std::ios::app);
  }

  void push(std::string&& mpi_fun) {
    trace_file << mpi_fun << std::endl;
    trace_file.flush();
  }

  void close() {
    if (trace_file.is_open()) {
      trace_file.close();
    }
  }

  ~StringTrace() {
    if (trace_file.is_open()) {
      trace_file.close();
    }
  }
};

struct CerrTrace {
  void open(int rank) {
  }

  void close() {
  }

  void push(std::string&& mpi_fun) {
    std::cerr << mpi_fun << std::endl;
  }
};

#ifndef MPITRACER_PRINT_TO_CONSOLE
using TraceObject = StringTrace;
#else
using TraceObject = CerrTrace;
#endif

TraceObject mpi_trace;

void mpi_arg_trace_push(const char* mpi_fn_name, const void* called_from) {
}

void mpi_arg_trace_exit(const char* mpi_fn_name, const void* called_from) {
  mpi_trace.close();
}

void mpi_arg_trace_start(const char* mpi_fn_name, const void* called_from) {
  int initialized{0};
  PMPI_Initialized(&initialized);
  if (initialized) {
    PMPI_Comm_rank(MPI_COMM_WORLD, &mpi_trace_process_rank);
  } else {
    std::cerr << "[MPI_ARG_TRACE] ERROR. Expected MPI to be initialzed in mpi_arg_trace_start. Called from '"
              << mpi_fn_name << "'" << std::endl;
  }
  mpi_trace.open(mpi_trace_process_rank);

  mpi_trace_target_file = []() {
    const auto mpi_target_file_env = getenv("MPI_ARG_TRACE_FILE_TARGET");
    if (mpi_target_file_env != nullptr) {
      const auto target_file_path = fs::path{mpi_target_file_env};
      if (is_regular_file(target_file_path)) {
        return target_file_path.string();
      }
    }
    return std::string{};
  }();

  const auto skip_env = getenv("MPI_ARG_TRACE_SKIP_HEADER");
  if (skip_env == nullptr) {
    mpi_trace.push(util::make_stream(",", "Function", "RANK", "TAG", "POLYXFER_NUM_ELEM_NNI", "DATATYPE",
                                     "COMMUNICATOR", "OPERATION", "newCOMMUNICATOR", "newDATATYPE", "file",
                                     "target-function", "line"));
  }
}

void mpi_arg_trace_push_full(
    const char* mpi_fun_name, const void* called_from, const int* ACCESS_MODE, const MPI_Aint* ALLOC_MEM_NUM_BYTES,
    const int* ARGUMENT_COUNT, char*** ARGUMENT_LIST, const int* ARRAY_LENGTH, const int* ARRAY_LENGTH_NNI,
    const int* ARRAY_LENGTH_PI, const int* ASSERT, const void* ATTRIBUTE_VAL, const void* ATTRIBUTE_VAL_10,
    const void* BUFFER, const int* COLOR, const MPI_Comm* COMMUNICATOR, const int* COMM_COMPARISON,
    const int* COMM_SIZE, const int* COORDINATE, const void* C_BUFFER, const void* C_BUFFER2,
    const MPI_Datatype* DATATYPE, const int* DEGREE, const int* DIMENSION, const MPI_Aint* DISPLACEMENT,
    const MPI_Errhandler* ERRHANDLER, const int* ERROR_CLASS, const int* ERROR_CODE, const int* EXTRA_STATE,
    const MPI_File* FILE,
    // const MPI_Win_copy_attr_function* FUNCTION,//TODO MPI_Win_copy_attr_function is not the only type possible here!
    const void* FUNCTION,  // TODO MPI_Win_copy_attr_function is not the only type possible here!
    const MPI_Group* GROUP, const int* INDEX, const MPI_Info* INFO, const int* INFO_VALUE_LENGTH, const int* KEY,
    const int* KEYVAL, const int* KEY_INDEX, const int* LOCK_TYPE, const int* LOGICAL, const MPI_Message* MESSAGE,
    const MPI_Count* NUM_BYTES, const int* NUM_DIMS, const MPI_Offset* OFFSET, const MPI_Op* OPERATION,
    const int* ORDER, const int* POLYDISPLACEMENT, const MPI_Aint* POLYDISPLACEMENT_AINT_COUNT,
    const int* POLYDISPLACEMENT_COUNT, const MPI_Aint* POLYDISPOFFSET, const void* POLYDTYPE_NUM_ELEM,
    const int* POLYDTYPE_NUM_ELEM_NNI, const int* POLYDTYPE_NUM_ELEM_PI, const MPI_Aint* POLYDTYPE_STRIDE_BYTES,
    // const MPI_Datarep_conversion_function* POLYFUNCTION,//TODO other types of functions are possible here
    const void* POLYFUNCTION, const int* POLYNUM_BYTES, const int* POLYNUM_BYTES_NNI, const int* POLYRMA_DISPLACEMENT,
    const int* POLYXFER_NUM_ELEM, const int* POLYXFER_NUM_ELEM_NNI, const int* PROFILE_LEVEL, const int* RANK,
    const int* RANK_NNI, const MPI_Request* REQUEST, const MPI_Aint* RMA_DISPLACEMENT_NNI, const int* SPLIT_TYPE,
    const MPI_Status* STATUS, const char* STRING, const int* STRING_LENGTH, const int* TAG, const int* THREAD_LEVEL,
    const int* TOPOLOGY_TYPE, const int* TYPECLASS, const int* TYPECLASS_SIZE, const int* UPDATE_MODE,
    const int* VERSION, const int* WEIGHT, const MPI_Win* WINDOW, const MPI_Aint* WINDOW_SIZE,
    const MPI_Aint* WIN_ATTACH_SIZE, const MPI_Comm* newCOMMUNICATOR, const MPI_Datatype* newDATATYPE,
    const MPI_Group* newGROUP, const MPI_Info* newINFO) {
  const auto sloc            = mpitracer::SourceLocation::create(called_from).value_or(SourceLocation{});
  const std::string mpi_file = mpi_trace_target_file.empty() ? sloc.file : mpi_trace_target_file;

  mpi_trace.push(util::make_stream(",", mpi_fun_name, RANK, TAG, POLYXFER_NUM_ELEM_NNI, util::mpi_datatype_t{DATATYPE},
                                   util::mpi_comm_t{COMMUNICATOR, mpi_fun_name}, util::mpi_op_t{OPERATION},
                                   util::mpi_comm_t{newCOMMUNICATOR, mpi_fun_name}, util::mpi_datatype_t{newDATATYPE},
                                   mpi_file, sloc.function, sloc.line));
}
