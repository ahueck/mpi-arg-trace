//
// Created by ahueck on 07.07.23.
//
#include "MPIUtil.h"

#include <mpi.h>
#include <optional>
#include <unordered_map>

namespace mpitracer::util::mpi {

struct MPICommTracker {
  std::unordered_map<MPI_Comm, std::string> table;
  MPICommTracker() {
    table.emplace(MPI_COMM_WORLD, "MPI_COMM_WORLD");
    table.emplace(MPI_COMM_SELF, "MPI_COMM_SELF");
    table.emplace(MPI_COMM_NULL, "MPI_COMM_NULL");
//    table.emplace(MPI_COMM_PARENT, "MPI_COMM_PARENT");
  }

  std::string get(const MPI_Comm comm) const {
    auto result = table.find(comm);
    if (result == std::end(table)) {
      return "";
    }
    return result->second;
  }

  bool push(std::string name, const MPI_Comm comm) {
    const auto result = table.emplace(comm, name);
    return result.second;
  }
};

namespace detail {
template <class... MPIFunStr>
bool mpi_creator_any_of(const std::string& lhs, MPIFunStr&&... rhs) {
  const auto starts_with = [](const std::string& str, std::string_view prefix) { return str.rfind(prefix, 0) == 0; };
  return !lhs.empty() && ((starts_with(lhs, std::forward<MPIFunStr>(rhs))) || ...);
}
}  // namespace detail

struct MPICommCreator {
  bool is_creator(const std::string& mpi_fun) const {
    // "MPI_Comm_split_type"
    return detail::mpi_creator_any_of(mpi_fun,                                                               //
                                      "MPI_Comm_dup", "MPI_Comm_idup", "MPI_Comm_create", "MPI_Comm_split",  //
                                      "MPI_Intercomm_create", "MPI_Intercomm_merge",                         //
                                      "MPI_Dist_graph_create",                                               //
                                      "MPI_Cart_create", "MPI_Cart_sub", "MPI_Cart_create",                  //
                                      "MPI_Graph_create");
  }
};

std::string named_comm_string(MPI_Comm comm) {
  char name[MPI_MAX_OBJECT_NAME];
  int namelen;
  auto mpierr = PMPI_Comm_get_name(comm, name, &namelen);
  if (mpierr != MPI_SUCCESS) {
    return {""};
  }
  return std::string{name};
}

std::string mpi_comm_name(const CommObject& comm) {
  static MPICommTracker comm_tracker;
  static MPICommCreator comm_create;

  MPI_Comm comm_ = *comm.get();
  //  std::cerr << "mpi_comm_name: " << comm.mpi_fun << "->"  << comm_ << " (\"" << comm_tracker.get(comm_) << "\")"  <<
  //  ":" << comm.get() << "\n";
  const auto name = comm_tracker.get(comm_);
  if (!name.empty()) {
    return name;
  }

  if (!comm.mpi_fun.empty() && comm_create.is_creator(comm.mpi_fun)) {
    const auto pushed = comm_tracker.push(comm.mpi_fun, comm_);
    //    std::cerr << "Pushed: " << std::noboolalpha << pushed << " -> "<< comm_ << " (\"" << comm_tracker.get(comm_)
    //    << "\")" << "\n";
    return comm.mpi_fun;
  }

  return "Unknown_MPI_Comm";
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

  if (NULL == dtype || MPI_DATATYPE_NULL == dtype) {
    return {};
  }
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