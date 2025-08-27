#pragma once

#include <map>

#include "mydocker/cgroup/subsystem.hh"
#include "mydocker/common/macro.hh"

namespace mydocker {

class CgroupManager;

class Cgroup {
  DISABLE_COPY_AND_ASSIGN(Cgroup);

 public:
  explicit Cgroup(const CgroupManager& manager, fs::path path)
      : cgroup_manager_(manager), cgroup_path_(std::move(path)) {}
  ~Cgroup();

  [[nodiscard]] Status ConfigureMemory(const MemoryConfig& mem_config);
  //   [[nodiscard]] bool ConfigureCPU(const std::string& cgroup_name,
  //                                   const CpuConfig& config);

  Status ResetMemoryConfiguration();
  Status ResetCpuConfiguration();
  Status Apply(pid_t pid);
  Status Destroy();

 private:
  Status MoveProcsToRoot();
  Status RemoveCgroup();

  const CgroupManager& cgroup_manager_;
  fs::path cgroup_path_;
  std::map<SubsystemType, std::unique_ptr<Subsystem>> subsystems_;
};

}  // namespace mydocker