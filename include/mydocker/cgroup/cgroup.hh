#pragma once

#include <map>

#include "mydocker/cgroup/subsystem.hh"
#include "mydocker/common/macro.hh"

namespace mydocker {

class Cgroup {
  DISABLE_COPY_AND_ASSIGN(Cgroup);
  DISABLE_MOVE_AND_ASSIGN(Cgroup);

 public:
  explicit Cgroup(fs::path path) : cgroup_path_(std::move(path)) {}

  [[nodiscard]] bool ConfigureMemory(const MemoryConfig& mem_config);
  //   [[nodiscard]] bool ConfigureCPU(const std::string& cgroup_name,
  //                                   const CpuConfig& config);

  void ResetMemoryConfiguration();
  void ResetCpuConfiguration();

 private:
  fs::path cgroup_path_;
  std::map<SubsystemType, std::unique_ptr<Subsystem>> subsystems_;
};

}  // namespace mydocker