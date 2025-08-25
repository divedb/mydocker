#pragma once

#include <string>

#include "mydocker/cgroup/subsystem.hh"

namespace mydocker {

class CgroupManager;  ///< Forward declaration

class MemorySubsystem : public Subsystem {
 public:
  explicit MemorySubsystem(std::string cgroup_path)
      : Subsystem(std::move(cgroup_path)) {}

  Status Apply(const ResourceConfig& rc) override;
  Status Reset() override;
  std::string_view Name() const { return "memory"; }

 private:
  Status ApplyLimit(const fs::path& file, size_t value);

  MemoryConfig cached_mem_config_;
};

}  // namespace mydocker