#ifndef MPI_ARG_TRACE_SYSTEM_H
#define MPI_ARG_TRACE_SYSTEM_H

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace mpitracer {

namespace system {
class Process {
  std::string self_exe;

  Process();

 public:
  static const Process& get();

  [[nodiscard]] const std::string& exe() const;
  [[nodiscard]] static long getMaxRSS();
};
}  // namespace system

struct SourceLocation {
  std::string function;
  std::string file;
  std::string line;

  static std::optional<SourceLocation> create(const void* addr, intptr_t offset_ptr = 1);
};

}  // namespace mpitracer

#endif  // MPI_ARG_TRACE_SYSTEM_H
