#include "mydocker/cgroup/cgroup_manager.hh"

#include <filesystem>

#include "mydocker/cgroup/cgroup_version.hh"

namespace fs = std::filesystem;

namespace mydocker {

CgroupManager::CgroupManager() {
  auto version = parse_cgroup_version();
  is_valid_ = is_cgroup_v2_mounted(version);
}

bool CgroupManager::IsValid() const { return is_valid_; }

bool CgroupManager::CreateDir(const std::string& dir) {
  if (!IsValid()) return false;

  fs::path cgroup_path = "/sys/fs/cgroup/";
  fs::create_directory(cgroup_path / dir);

  try {
    fs::create_directories(cgroup_path);
    return true;  // Successfully opened the cgroup directory
  } catch (const fs::filesystem_error& e) {
    return false;  // Failed to open the directory
  }
}

}  // namespace mydocker