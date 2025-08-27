#include "mydocker/cgroup/cgroup.hh"

#include <glog/logging.h>

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
  MoveProcsToRoot();
  RemoveCgroup();
}

Status Cgroup::MoveProcsToRoot() {
  // splice?
  // Move the process to the root.
  auto root_cgroup = cgroup_manager_.CgroupMountPath() / "cgroup.procs";
  std::ofstream ofs(root_cgroup);

  if (!ofs.is_open()) {
    LOG(ERROR) << "failed to open " << root_cgroup.string();
    return;
  }

  std::ifstream ifs(cgroup_path_);

  if (!ifs.is_open()) {
    LOG(ERROR) << "failed to open " << cgroup_path_.string();
    return;
  }

  std::string pid;

  while (std::getline(ifs, pid)) {
    ofs << pid << '\n';
  }
}

Status Cgroup::RemoveCgroup() {
  std::error_code ec;

  if (!fs::remove(cgroup_path_, ec) || ec) {
    LOG(ERROR) << "failed to remove '" << cgroup_path_.string()
               << "':" << ec.message();
  }
}

Status Cgroup::Apply(pid_t pid) {}

}  // namespace mydocker