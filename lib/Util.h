#ifndef MPITRACER_UTIL_H
#define MPITRACER_UTIL_H

#include <functional>
#include <mutex>
#include <shared_mutex>

namespace mpitracer::util {

struct MutexSyncMixin {
  mutable std::shared_mutex mtx;

  template <typename Callable, typename... Args>
  auto lock_and_call(Callable&& func, Args&&... args) const {
    std::unique_lock lock(mtx);
    return std::invoke(std::forward<Callable>(func), std::forward<Args>(args)...);
  }

  template <typename Callable, typename... Args>
  auto readlock_and_call(Callable&& func, Args&&... args) const {
    std::shared_lock lock(mtx);
    return std::invoke(std::forward<Callable>(func), std::forward<Args>(args)...);
  }
};

}  // namespace mpitracer::util

#endif  // MPITRACER_UTIL_H
