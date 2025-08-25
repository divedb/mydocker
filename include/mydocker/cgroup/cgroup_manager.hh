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
  explicit CgroupManager(const char* program_name);
  ~CgroupManager();

  [[no_discard]] bool CreateCgroup(const std::string& cgroup_name);
  void RemoveCgroup(const std::string& cgroup_name);

  [[no_discard]] bool ConfigureMemory(const std::string& cgroup_name,
                                      const MemoryConfig& config);
  [[no_discard]] bool ConfigureCPU(const std::string& cgroup_name,
                                   const CpuConfig& config);
  void ResetMemoryConfiguration(const std::string& cgroup_name);
  void ResetCpuConfiguration(const std::string& cgroup_name);

 private:
  bool GetSubsystem(const std::string& cgroup_name, Subsystem* sys) const;

  bool cgroup_v2_mounted_;
  const fs::path cgroup_mount_path_ = "/sys/fs/cgroup/";
  std::map<std::string, std::unique_ptr<Subsystem>> cgroup_name_to_subs_;
};

}  // namespace mydocker