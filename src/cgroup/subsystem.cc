#include "mydocker/cgroup/subsystem.hh"

#include "mydocker/cgroup/memory.hh"

namespace mydocker {

std::unique_ptr<Subsystem> make_default_subsystem(const fs::path& cgroup_path,
                                                  SubsystemType stype) {
  if (stype == kMemory) return std::make_unique<MemorySubsystem>(cgroup_path);

  std::exit(-2);
}

}  // namespace mydocker