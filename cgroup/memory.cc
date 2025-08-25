#include "mydocker/cgroup/memory.hh"

#include <glog/logging.h>

#include <fstream>

namespace mydocker {

Status MemorySubsystem::Apply(const ResourceConfig& rc) {
  const MemoryConfig& mem_config = rc.mem_config;

  // No changes needed.
  if (cached_mem_config_ == mem_config) return Status::OK();

  auto apply_if_changed = [&](size_t MemoryConfig::*mptr,
                              const std::string& filename) -> Status {
    if (mem_config.*mptr != cached_mem_config_.*mptr)
      return ApplyLimit(cgroup_path_ / filename, mem_config.*mptr);

    return Status::OK();
  };

  if (Status s = apply_if_changed(&MemoryConfig::max_in_bytes, "memory.max");
      !s)
    return s;

  if (Status s = apply_if_changed(&MemoryConfig::high_in_bytes, "memory.high");
      !s)
    return s;

  if (Status s = apply_if_changed(&MemoryConfig::min_in_bytes, "memory.min");
      !s)
    return s;

  if (Status s = apply_if_changed(&MemoryConfig::low_in_bytes, "memory.low");
      !s)
    return s;

  // Cache the configuration.
  cached_mem_config_ = mem_config;

  return Status::OK();
}

Status MemorySubsystem::ApplyLimit(const fs::path& file, size_t value) {
  std::ofstream ofs(file);

  if (!ofs.is_open()) return Status::Error("Failed to open " + file.string());

  if (value == static_cast<size_t>(-1))
    ofs << "max";
  else
    ofs << value;

  if (!ofs.good())
    return Status::Error("Failed to write value to " + file.string());

  return Status::OK();
}

}  // namespace mydocker