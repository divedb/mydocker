#include "mydocker/cgroup/cgroup.hh"

#include "mydocker/cgroup/memory.hh"

namespace mydocker {

bool Cgroup::ConfigureMemory(const MemoryConfig& mem_config) {
  auto [it, inserted] = subsystems_.try_emplace(
      kMemory, std::make_unique<MemorySubsystem>(cgroup_path_));

  return it->second->Apply({.mem_config = mem_config});
}

void Cgroup::ResetMemoryConfiguration() {
  auto it = subsystems_.find(kMemory);

  if (it == subsystems_.end()) return;

  it->second->Reset();
}

void Cgroup::ResetCpuConfiguration() {}

}  // namespace mydocker