#pragma once

#include <string>

#include "mydocker/cgroup/subsystem.hh"

namespace mydocker {

class CgroupManager;  ///< Forward declaration

class MemorySubsystem : public Subsystem {
 public:
  explicit MemorySubsystem(std::string cgroup_path)
      : Subsystem(std::move(cgroup_path)) {}

  bool Apply(const ResourceConfig& rc) override;
  bool Reset() override;
  const std::string& Name() const { return "memory"; }

 private:
  void ApplyLimit(const fs::path& file, size_t value);

  MemoryConfig cached_mem_config_;
};

}  // namespace mydocker