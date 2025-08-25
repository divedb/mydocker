#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "mydocker/cgroup/subsystem.hh"

namespace mydocker {

class CpuConfig;
class MemoryConfig;

class CgroupManager {
 public:
  CgroupManager();
  ~CgroupManager();

  [[nodiscard]] bool CreateCgroup(const std::string& cgroup_name);
  bool RemoveCgroup(const std::string& cgroup_name);

  [[nodiscard]] bool ConfigureMemory(const std::string& cgroup_name,
                                     const MemoryConfig& config);
  [[nodiscard]] bool ConfigureCPU(const std::string& cgroup_name,
                                  const CpuConfig& config);
  void ResetMemoryConfiguration(const std::string& cgroup_name);
  void ResetCpuConfiguration(const std::string& cgroup_name);

  const fs::path& CgroupMountPath() const { return cgroup_mount_path_; }

 private:
  fs::path GetCgroupPath(const std::string& cgroup_name) const;
  bool GetSubsystem(const std::string& cgroup_name, Subsystem*& sys) const;
  bool RemoveCgroupDir(const fs::path& cgroup_path);

  bool cgroup_v2_mounted_;
  const fs::path cgroup_mount_path_ = "/sys/fs/cgroup/";
  std::map<std::string, std::unique_ptr<Subsystem>> cgroup_name_to_subs_;
};

}  // namespace mydocker