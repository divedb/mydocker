#include "mydocker/cgroup/cgroup_manager.hh"

#include <glog/logging.h>

#include <cassert>
#include <filesystem>

#include "mydocker/cgroup/cgroup_version.hh"
#include "mydocker/cgroup/memory.hh"

#define CHECK_CGROUP_V2_IS_SUPPORTED()             \
  do {                                             \
    if (!cgroup_v2_mounted_) {                     \
      LOG(WARNING) << "Cgroup v2 is not mounted."; \
      return false;                                \
    }                                              \
  } while (0)

namespace mydocker {

CgroupManager::CgroupManager(const char* program_name) {
  google::InitGoogleLogging(program_name);
  auto version = parse_cgroup_version();
  cgroup_v2_mounted_ = is_cgroup_v2_mounted(version);
}

CgroupManager::~CgroupManager() {
  for (auto& [name, sub] : cgroup_name_to_subs_) RemoveCgroup(name);
}

bool CgroupManager::CreateCgroup(const std::string& cgroup_name) {
  CHECK_CGROUP_V2_IS_SUPPORTED();

  std::error_code ec;
  auto const cgroup_path = cgroup_mount_path_ / cgroup_name;

  // Deletes the contents of p (if it is a directory) and the contents of all
  // its subdirectories, recursively, then deletes p itself as if by repeatedly
  // applying the POSIX remove. Symlinks are not followed (symlink is removed,
  // not its target).
  if (fs::exists(cgroup_path)) {
    if (fs::remove_all(cgroup_path, ec) == static_cast<uintmax_t>(-1) || ec) {
      LOG(ERROR) << "Failed to remove existing cgroup directory '"
                 << cgroup_path << "': " << ec.message();

      return false;
    }
  }

  if (!fs::create_directory(cgroup_path, ec) || ec) {
    LOG(ERROR) << "Failed to create cgroup directory '" << cgroup_path
               << "': " << ec.message();

    return false;
  }

  cgroup_name_to_subs_.emplace(cgroup_name,
                               std::make_unique<MemorySubsystem>(cgroup_path));

  return true;
}

void CgroupManager::RemoveCgroup(const std::string& cgroup_name) {
  std::error_code ec;
  auto const cgroup_path = cgroup_mount_path_ / cgroup_name;

  if (fs::remove_all(cgroup_path, ec) == static_cast<uintmax_t>(-1) || ec) {
    LOG(WARNING) << "Failed to remove cgroup directory '" << cgroup_name
                 << "': " << ec.message();

    return;
  }

  cgroup_name_to_subs_.erase(cgroup_name);
}

bool CgroupManager::ConfigureMemory(const std::string& cgroup_name,
                                    const MemoryConfig& config) {
  CHECK_CGROUP_V2_IS_SUPPORTED();

  Subsystem* sys;

  if (!GetSubsystem(cgroup_name, sys)) return false;

  return sys->Apply({.mem_config = config});
}

bool CgroupManager::GetSubsystem(const std::string& cgroup_name,
                                 Subsystem* sys) const {
  auto it = cgroup_name_to_subs_.find(cgroup_name);

  if (it == cgroup_name_to_subs_.end()) {
    LOG(WARNING) << "Create cgroup before making any configuration.";

    return false;
  }

  sys = it->second.get();

  return true;
}

}  // namespace mydocker