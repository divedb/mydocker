#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include "mydocker/common/status.hh"

namespace fs = std::filesystem;

namespace mydocker {

struct MemoryConfig {
  size_t max_in_bytes = -1;   ///< Default: no limit
  size_t high_in_bytes = -1;  ///< Default: no limit
  size_t min_in_bytes = 0;    ///< Default: no minimum
  size_t low_in_bytes = 0;    ///< Default: no minimum

  constexpr bool operator==(const MemoryConfig&) const = default;
};

struct ResourceConfig {
  MemoryConfig mem_config;
};

class Subsystem {
 public:
  virtual ~Subsystem() = default;

  explicit Subsystem(fs::path cgroup_path)
      : cgroup_path_(std::move(cgroup_path)) {}

  Subsystem(const Subsystem&) = default;
  Subsystem(Subsystem&&) = default;
  Subsystem& operator=(const Subsystem&) = default;
  Subsystem& operator=(Subsystem&&) = default;

  virtual void Init() {}
  virtual Status Apply(const ResourceConfig& rc) = 0;
  virtual Status Reset() = 0;
  virtual std::string_view Name() const = 0;

 protected:
  fs::path cgroup_path_;  ///< Path to the cgroup directory
};

enum SubsystemType : int { kMemory = 0, kCpu = 1, kMaxSubsystem };

}  // namespace mydocker