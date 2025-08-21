#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace mydocker {

enum class CgroupVersion : uint8_t {
  kV1 = 0x01,
  kV2 = 0x02,
  kHybrid = kV1 | kV2,
  kUnknown = 0x00
};

inline CgroupVersion ParseMountInfoLine(const std::string &line) {
  auto pos = line.find(" - ");

  if (pos == std::string::npos) return CgroupVersion::kUnknown;

  // pattern: fs_type mount_source super_options
  auto pattern = line.substr(pos + 3);
  std::istringstream iss(pattern);
  std::string fs_type;

  if (!(iss >> fs_type)) return CgroupVersion::kUnknown;

  if (fs_type == "cgroup")
    return CgroupVersion::kV1;
  else if (fs_type == "cgroup2")
    return CgroupVersion::kV2;
  else
    return CgroupVersion::kUnknown;
}

inline CgroupVersion GetCgroupVersion() {
  std::ifstream mountinfo("/proc/self/mountinfo");

  if (!mountinfo.is_open()) return CgroupVersion::kUnknown;

  std::string line;
  bool has_v1 = false;
  bool has_v2 = false;

  while (std::getline(mountinfo, line)) {
    auto version = ParseMountInfoLine(line);

    if (version == CgroupVersion::kV1) has_v1 = true;
    if (version == CgroupVersion::kV2) has_v2 = true;

    if (has_v1 && has_v2) break;
  }

  if (has_v1 && has_v2) return CgroupVersion::kHybrid;
  if (has_v1) return CgroupVersion::kV1;
  if (has_v2) return CgroupVersion::kV2;

  return CgroupVersion::kUnknown;
}

}  // namespace mydocker