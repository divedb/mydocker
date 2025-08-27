#pragma once

#include <array>
#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "mydocker/cgroup/cgroup.hh"

namespace mydocker {

class CpuConfig;
class MemoryConfig;

class CgroupManager {
 public:
  CgroupManager();

  [[nodiscard]] bool CreateCgroup(const std::string& cgroup_name);
  bool RemoveCgroup(const std::string& cgroup_name);

  [[nodiscard]] bool ConfigureMemory(const std::string& cgroup_name,
                                     const MemoryConfig& mem_config);
  [[nodiscard]] bool ConfigureCPU(const std::string& cgroup_name,
                                  const CpuConfig& config);
  void ResetMemoryConfiguration(const std::string& cgroup_name);
  void ResetCpuConfiguration(const std::string& cgroup_name);

  [[nodiscard]] bool Apply(const std::string& cgroup_name, pid_t pid);

  const fs::path& CgroupMountPath() const { return cgroup_mount_path_; }

 private:
  fs::path CgroupAbsPath(const std::string& cgroup_name) const {
    return cgroup_mount_path_ / cgroup_name;
  }

  bool GetCgroup(const std::string& cgroup_name, Cgroup*& cgroup,
                 bool verbose = true) const;

  bool cgroup_v2_mounted_;
  const fs::path cgroup_mount_path_ = "/sys/fs/cgroup/";

  /// The key represents a directory under /sys/fs/cgroup/, e.g., "mydir" or
  /// "subgroup/mydir".
  std::map<std::string, std::unique_ptr<Cgroup>> name_to_cgroups_;
};

}  // namespace mydocker