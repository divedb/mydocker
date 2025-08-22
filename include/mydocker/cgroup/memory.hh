#pragma once

#include <string>

namespace mydocker {

struct MemoryConfig {
  std::string max_in_bytes = "max";   ///< Default: no limit
  std::string high_in_bytes = "max";  ///< Default: no limit
  std::string min_in_bytes = "0";     ///< Default: no minimum
  std::string low_in_bytes = "0";     ///< Default: no minimum
};

class CgroupManager;  ///< Forward declaration

class MemorySubsystem {
 public:
  MemorySubsystem() = default;
  ~MemorySubsystem() = default;

  // Initialize the memory subsystem
  bool Init();

  // Apply the memory configuration to the cgroup
  bool Apply(const MemoryConfig& config);

  // Remove the cgroup
  bool Remove();

  // Get the name of the subsystem
  std::string Name() const { return "memory"; }

 private:
  std::string cgroup_path_;  ///< Path to the cgroup directory
};

}  // namespace mydocker