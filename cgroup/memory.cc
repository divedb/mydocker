#include "mydocker/cgroup/memory.hh"

#include <glog/logging.h>

#include <fstream>

namespace mydocker {

bool MemorySubsystem::Apply(const ResourceConfig& rc) {
  const MemoryConfig& mem_config = rc.mem_config;

  // No changes needed.
  if (cached_mem_config_ == mem_config) return true;

  if (mem_config.max_in_bytes != cached_mem_config_.max_in_bytes)
    ApplyLimit(cgroup_path_ / "memory.max", mem_config.max_in_bytes);

  if (mem_config.high_in_bytes != cached_mem_config_.high_in_bytes)
    ApplyLimit(cgroup_path_ / "memory.high", mem_config.high_in_bytes);

  if (mem_config.min_in_bytes != cached_mem_config_.min_in_bytes)
    ApplyLimit(cgroup_path_ / "memory.min", mem_config.min_in_bytes);

  if (mem_config.low_in_bytes != cached_mem_config_.low_in_bytes)
    ApplyLimit(cgroup_path_ / "memory.low", mem_config.low_in_bytes);

  // Cache the configuration.
  cached_mem_config_ = mem_config;

  return true;
}

void MemorySubsystem::ApplyLimit(const fs::path& file, size_t value) {
  std::ofstream ofs(file);

  if (!ofs.is_open()) {
    LOG(ERROR) << "Failed to open " << file;

    return;
  }

  if (value == static_cast<size_t>(-1))
    ofs << "max";
  else
    ofs << value;

  ofs.close();
}

}  // namespace mydocker