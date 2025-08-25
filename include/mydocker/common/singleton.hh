#pragma once

#include <memory>
#include <mutex>

namespace mydocker {

template <typename T>
class Singleton {
 public:
  template <typename... Args>
  static T& Instance() {
    std::call_once(flag_,
                   [&] { ptr_.reset(new T(std::forward<Args>(args)...)); });

    return *ptr_;
  }

 private:
  Singleton() = delete;
  ~Singleton() = delete;
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

  static inline std::unique_ptr<T> ptr_;
  static inline std::once_flag flag_;
};

}  // namespace mydocker