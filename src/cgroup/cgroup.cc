#include "mydocker/cgroup/cgroup.hh"

#include <errno.h>
#include <glog/logging.h>
#include <string.h>

#include <fstream>

#include "mydocker/cgroup/cgroup_manager.hh"
#include "mydocker/cgroup/memory.hh"

namespace mydocker {

Cgroup::~Cgroup() { Destroy(); }

Status Cgroup::ConfigureMemory(const MemoryConfig& mem_config) {
  auto [it, _] = subsystems_.try_emplace(
      kMemory, std::make_unique<MemorySubsystem>(cgroup_path_));

  return it->second->Apply({.mem_config = mem_config});
}

Status Cgroup::ResetMemoryConfiguration() {
  auto it = subsystems_.find(kMemory);

  if (it == subsystems_.end()) return Status::OK();

  return it->second->Reset();
}

Status Cgroup::ResetCpuConfiguration() { return Status::OK(); }

Status Cgroup::Destroy() {
  Status s1 = MoveProcsToRoot();
  Status s2 = RemoveCgroup();

  if (s1.HasError()) return s1;

  return s2;
}

Status Cgroup::MoveProcsToRoot() {
  // splice?
  // Move the process to the root.
  auto root_cgroup = cgroup_manager_.CgroupMountPath() / "cgroup.procs";
  std::ofstream ofs(root_cgroup);

  if (!ofs.is_open())
    return Status::Error("failed to open " + root_cgroup.string());

  std::ifstream ifs(cgroup_path_);

  if (!ifs.is_open())
    return Status::Error("failed to open " + cgroup_path_.string());

  std::string pid;

  while (std::getline(ifs, pid)) {
    ofs << pid << '\n';
  }

  return Status::OK();
}

Status Cgroup::RemoveCgroup() {
  std::error_code ec;

  if (!fs::remove(cgroup_path_, ec) || ec)
    return Status::Error("failed to remove '" + cgroup_path_.string() +
                         "':" + ec.message());

  return Status::OK();
}

Status Cgroup::Apply(pid_t pid) {
  fs::path path = cgroup_path_ / "cgroup.procs";
  std::ofstream ofs(path);

  if (!ofs.is_open()) return Status::Error("failed to open " + path.string());

  ofs << pid;

  if (!ofs)
    return Status::Error("failed to write pid " + std::to_string(pid) + " to " +
                         path.string() + ": " + strerror(errno));

  return Status::OK();
}

}  // namespace mydocker