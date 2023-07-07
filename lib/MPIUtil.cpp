//
// Created by ahueck on 07.07.23.
//
#include "MPIUtil.h"

#include <mpi.h>
#include <optional>

namespace mpitracer::util::mpi {

std::string mpi_comm_name(MPI_Comm comm) {
  if (comm == MPI_COMM_WORLD) {
    return "MPI_COMM_WORLD";
  }
  if (comm == MPI_COMM_SELF) {
    return "MPI_COMM_SELF";
  }
  if (comm == MPI_COMM_NULL) {
    return "MPI_COMM_NULL";
  }

  return "User-defined_MPI_Comm";
}

std::string combiner_name_for(int combiner) {
  switch (combiner) {
    case MPI_COMBINER_NAMED:
      return "MPI_COMBINER_NAMED";
    case MPI_COMBINER_DUP:
      return "MPI_Type_dup";
    case MPI_COMBINER_CONTIGUOUS:
      return "MPI_Type_contiguous";
    case MPI_COMBINER_VECTOR:
      return "MPI_Type_vector";
    case MPI_COMBINER_HVECTOR:
      return "MPI_Type_create_hvector";
    case MPI_COMBINER_INDEXED:
      return "MPI_Type_indexed";
    case MPI_COMBINER_HINDEXED:
      return "MPI_Type_create_hindexed";
    case MPI_COMBINER_INDEXED_BLOCK:
      return "MPI_Type_create_indexed_block";
    case MPI_COMBINER_STRUCT:
      return "MPI_Type_create_struct";
    case MPI_COMBINER_SUBARRAY:
      return "MPI_Type_create_subarray";
    case MPI_COMBINER_DARRAY:
      return "MPI_Type_create_darray";
    case MPI_COMBINER_F90_REAL:
      return "MPI_Type_create_f90_real";
    case MPI_COMBINER_F90_COMPLEX:
      return "MPI_Type_create_f90_complex";
    case MPI_COMBINER_F90_INTEGER:
      return "MPI_Type_create_f90_integer";
    case MPI_COMBINER_RESIZED:
      return "MPI_Type_create_resized";
    case MPI_COMBINER_HINDEXED_BLOCK:
      return "MPI_Type_create_hindexed_block";
    default:
      return "invalid_combiner_id";
  }
}

std::string named_combiner_string(MPI_Datatype dtype) {
  //  MPI_UNSIGNED_CHAR
  char name[MPI_MAX_OBJECT_NAME];
  int namelen;
  auto mpierr = PMPI_Type_get_name(dtype, name, &namelen);
  if (mpierr != MPI_SUCCESS) {
    return {""};
  }
  return std::string{name};
}

std::optional<int> get_combiner_id(MPI_Datatype dtype) {
  int num_integers;
  int num_addresses;
  int num_datatypes;
  int combiner;
  auto mpierr = PMPI_Type_get_envelope(dtype, &num_integers, &num_addresses, &num_datatypes, &combiner);

  if (mpierr != MPI_SUCCESS) {
    return {};
  }
  return combiner;
}

std::string mpi_op_name(MPI_Op operation) {
  if (operation == MPI_MAX) {
    return "MPI_MAX";
  } else if (operation == MPI_MIN) {
    return "MPI_MIN";
  } else if (operation == MPI_SUM) {
    return "MPI_SUM";
  } else if (operation == MPI_PROD) {
    return "MPI_PROD";
  } else if (operation == MPI_LAND) {
    return "MPI_LAND";
  } else if (operation == MPI_BAND) {
    return "MPI_BAND";
  } else if (operation == MPI_LOR) {
    return "MPI_LOR";
  } else if (operation == MPI_BOR) {
    return "MPI_BOR";
  } else if (operation == MPI_LXOR) {
    return "MPI_LXOR";
  } else if (operation == MPI_BXOR) {
    return "MPI_BXOR";
  } else if (operation == MPI_MAXLOC) {
    return "MPI_MAXLOC";
  } else if (operation == MPI_MINLOC) {
    return "MPI_MINLOC";
  } else if (operation == MPI_REPLACE) {
    return "MPI_REPLACE";
  } else if (operation == MPI_NO_OP) {
    return "MPI_NO_OP";
  }
  return "User-defined_MPI_Op";
}

}  // namespace mpitracer::util::mpi