#ifndef MPI_ARG_TRACE_UTIL_H
#define MPI_ARG_TRACE_UTIL_H

#include "System.h"

#include <mpi.h>
#include <sstream>

namespace mpitracer {
namespace util {

namespace detail {
template <typename T, typename Parameter>
class NamedType {
 public:
  explicit NamedType(T const& value) : value_(value) {
  }
  explicit NamedType(T&& value) : value_(std::move(value)) {
  }

  T& get() {
    return value_;
  }
  T const& get() const {
    return value_;
  }

 private:
  T value_;
};

template <typename T, typename Parameter>
struct NamedMPIType final : public NamedType<T, Parameter> {
 private:
  std::string_view mpi_fun_{};
  bool new_value_{false};

 public:
  explicit NamedMPIType(T const& value, std::string_view mpi_fun = "", bool new_val = false)
      : NamedType<T, Parameter>(value), mpi_fun_(mpi_fun), new_value_(new_val) {
  }
  explicit NamedMPIType(T&& value, std::string_view mpi_fun = "", bool new_val = false)
      : NamedType<T, Parameter>(std::move(value)), mpi_fun_(mpi_fun), new_value_(new_val) {
  }

  std::string_view mpi_function() const {
    return mpi_fun_;
  };

  bool is_initialized() const {
    return !new_value_;
  }
};

}  // namespace detail

using mpi_datatype_t = detail::NamedMPIType<const MPI_Datatype*, struct MPIDtype>;
using mpi_comm_t     = detail::NamedMPIType<const MPI_Comm*, struct MPICom>;
using mpi_op_t       = detail::NamedMPIType<const MPI_Op*, struct MPIOp>;

namespace mpi {
std::string combiner_name_for(int combiner);
std::optional<int> get_combiner_id(MPI_Datatype dtype);
std::string named_combiner_string(MPI_Datatype dtype);
std::string mpi_op_name(MPI_Op operation);
std::string mpi_comm_name(const mpi_comm_t& operation);
}  // namespace mpi

namespace detail {
template <class T>
struct array_to_pointer_decay {
  typedef T type;
};

template <class T, std::size_t N>
struct array_to_pointer_decay<T[N]> {
  typedef const T* type;
};

template <typename T>
struct ForSpecialization {
  static auto value(const T& v) {
    if constexpr (std::is_arithmetic_v<std::remove_pointer_t<T>>) {
      if (v != nullptr) {
        return std::to_string(*v);
      }
    }
    return std::string{""};
  }
};

template <>
struct ForSpecialization<mpi_comm_t> {
  static auto value(const mpi_comm_t& comm) {
    if (comm.get() == nullptr) {
      return std::string{""};
    }
    //    const auto* communicator = comm.get();
    return mpi::mpi_comm_name(comm);
  }
};

template <>
struct ForSpecialization<mpi_op_t> {
  static auto value(const mpi_op_t& op) {
    if (op.get() == nullptr) {
      return std::string{""};
    }
    const auto* operation = op.get();
    return std::string{mpi::mpi_op_name(*operation)};
  }
};

template <>
struct ForSpecialization<mpi_datatype_t> {
  static auto value(const mpi_datatype_t& dtype) {
    if (dtype.get() == nullptr) {
      return std::string{""};
    }
    if (!dtype.is_initialized()) {
      return std::string{dtype.mpi_function()};
    }
    const auto* datatype = dtype.get();
    auto combiner        = mpi::get_combiner_id(*datatype);
    if (combiner.has_value()) {
      if (combiner.value() == MPI_COMBINER_NAMED) {
        return mpi::named_combiner_string(*datatype);
      }
    }
    return std::string{mpi::combiner_name_for(combiner.value_or(-1))};
  }
};

template <>
struct ForSpecialization<const char*> {
  static auto value(const char* v) {
    return std::string{v};
  }
};

template <>
struct ForSpecialization<std::string> {
  static auto value(const std::string& v) {
    return v;
  }
};
} /* namespace detail */

template <typename T>
auto value(const T& v) {
  typedef typename detail::array_to_pointer_decay<T>::type S_Ty;
  return detail::ForSpecialization<S_Ty>::value(v);
}

template <typename... Ts>
void apply_ostream(std::ostream& out, std::string_view separator, Ts&&... values) {
  ((out << value(std::forward<Ts>(values)) << separator), ...);
}

template <typename... Ts>
std::string make_stream(std::string_view separator, Ts&&... values) {
  std::ostringstream ostream;
  util::apply_ostream(ostream, separator, std::forward<Ts>(values)...);
  std::string content = ostream.str();

  const auto erase_last = [&]() {
    const auto last_pos = content.find_last_of(separator);
    if (last_pos != std::string::npos) {
      content.erase(last_pos, 1);
    }
  };

  erase_last();

  return content;
}

}  // namespace util
}  // namespace mpitracer

#endif  // MPI_ARG_TRACE_UTIL_H
