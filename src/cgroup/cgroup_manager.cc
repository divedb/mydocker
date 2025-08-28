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

CgroupManager::CgroupManager() {
  auto version = parse_cgroup_version();
  cgroup_v2_mounted_ = is_cgroup_v2_mounted(version);
}

CgroupManager::~CgroupManager() {
  for (auto& [_, cgroup] : name_to_cgroups_) {
    Status status = cgroup->Destroy();

    if (status.HasError()) LOG(ERROR) << status.Message();
  }
}

bool CgroupManager::CreateCgroup(const std::string& cgroup_name) {
  CHECK_CGROUP_V2_IS_SUPPORTED();

  if (Cgroup * cgroup; GetCgroup(cgroup_name, cgroup, false)) {
    LOG(WARNING) << cgroup_name << " has existed.";

    return true;
  }

  std::error_code ec;
  auto const cgroup_path = CgroupAbsPath(cgroup_name);

  // Deletes the contents of p (if it is a directory) and the contents of all
  // its subdirectories, recursively, then deletes p itself as if by repeatedly
  // applying the POSIX remove. Symlinks are not followed (symlink is removed,
  // not its target).
  if (!fs::exists(cgroup_path)) {
    if (!fs::create_directory(cgroup_path, ec) || ec) {
      LOG(ERROR) << "failed to create cgroup directory '" << cgroup_path
                 << "': " << ec.message();

      return false;
    }
  }

  name_to_cgroups_.emplace(
      cgroup_name, std::make_unique<Cgroup>(*this, std::move(cgroup_path)));

  return true;
}

bool CgroupManager::RemoveCgroup(const std::string& cgroup_name) {
  auto it = name_to_cgroups_.find(cgroup_name);

  if (it == name_to_cgroups_.end()) {
    LOG(WARNING) << "failed to remove cgroup name '" << cgroup_name
                 << "' that doesn't exist.";

    return false;
  }

  name_to_cgroups_.erase(it);

  return true;
}

bool CgroupManager::ConfigureMemory(const std::string& cgroup_name,
                                    const MemoryConfig& mem_config) {
  CHECK_CGROUP_V2_IS_SUPPORTED();

  if (Cgroup * cgroup; GetCgroup(cgroup_name, cgroup))
    return cgroup->ConfigureMemory(mem_config);

  return false;
}

bool CgroupManager::GetCgroup(const std::string& cgroup_name, Cgroup*& cgroup,
                              bool verbose) const {
  auto it = name_to_cgroups_.find(cgroup_name);

  if (it == name_to_cgroups_.end()) {
    if (verbose)
      LOG(WARNING) << cgroup_name << " doesn't exist and create it first.";

    return false;
  }

  cgroup = it->second.get();

  return true;
}

[[nodiscard]] bool CgroupManager::Apply(const std::string& cgroup_name,
                                        pid_t pid) {
  CHECK_CGROUP_V2_IS_SUPPORTED();

  Cgroup* cgroup;

  if (!GetCgroup(cgroup_name, cgroup)) return false;

  Status status = cgroup->Apply(pid);

  if (status.HasError()) {
    LOG(ERROR) << status.Message();

    return false;
  }

  return true;
}

}  // namespace mydocker